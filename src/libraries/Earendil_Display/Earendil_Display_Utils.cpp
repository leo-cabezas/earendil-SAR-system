 #include <Earendil_Display.h>   // ATTENTION: Add all library dependencies inside this header.

namespace Earendil_Display {
    
    Adafruit_GC9A01A display = Adafruit_GC9A01A(TFT_CS, TFT_DC, TFT_RST);

    volatile Display_UI_t previous_ui;
    volatile Display_UI_t active_ui;
    NavScreenState_t NavState;
    
    // =======================================================================================
    // SETUP UTILITIES
    // =======================================================================================

    void setup(){
        setupMenuButtons();
        setupDisplay();
    }

    void setupDisplay(){
        display.begin();
        display.setRotation(0);
        display.fillScreen(GC9A01A_GREEN);
        display.setCursor(120, 120);
        display.setTextColor(GC9A01A_WHITE);

        previous_ui = NULL_UI;
        active_ui   = NAVIGATION_UI;
    }
    
    void setupMenuButtons(void){
        setupButton(BUTTON1);
        setupButton(BUTTON_BACK);
        setupButton(BUTTON_SELECT);
        setupButton(BUTTON_DOWN);
        setupButton(BUTTON_UP);

        irq_add_shared_handler(IO_IRQ_BANK0, IO_IRQ_BANK0_Handler, PICO_SHARED_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY);
        irq_set_enabled(IO_IRQ_BANK0, true);
    }

    void setupButton(const uint8_t BUTTON_PIN){
        gpio_init(BUTTON_PIN);
        gpio_set_dir(BUTTON_PIN, GPIO_IN);
        gpio_pull_up(BUTTON_PIN);
        gpio_set_irq_enabled(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true);
    }

    // VERY IMPORTANT: SHOULDN'T HAVE NULL AS THE SECOND ARGUMENT OF vTaskNotifyGiveFromISR. READ ON 
    inline void interruptHandler_BUTTON(const uint8_t BUTTON_PIN){
        gpio_set_irq_enabled(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, false);
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(Earendil_Handles->Display_Handles.task_vDisplay_Controls, NULL);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    void IO_IRQ_BANK0_Handler(void){
        if (gpio_get_irq_event_mask(BUTTON1)        & GPIO_IRQ_EDGE_FALL){
            interruptHandler_BUTTON(BUTTON1);
        }
        if (gpio_get_irq_event_mask(BUTTON_BACK)    & GPIO_IRQ_EDGE_FALL){
            interruptHandler_BUTTON(BUTTON_BACK);
        }
        if (gpio_get_irq_event_mask(BUTTON_SELECT)  & GPIO_IRQ_EDGE_FALL){
            interruptHandler_BUTTON(BUTTON_SELECT);
        }
        if (gpio_get_irq_event_mask(BUTTON_DOWN)    & GPIO_IRQ_EDGE_FALL){
            interruptHandler_BUTTON(BUTTON_DOWN);
        }
        if (gpio_get_irq_event_mask(BUTTON_UP)      & GPIO_IRQ_EDGE_FALL){
            interruptHandler_BUTTON(BUTTON_UP);
        }
    }

    // TEMPORARY !!!!!!!!!!!!!!!
    void request_sendPing_TX(){
        xTaskNotify(Earendil_Handles->Radio_Handles.task_vRadio_Ping_TX, 0, eNoAction);
    }

    void request_Magnetometer_Calibrate(){
        active_ui = CALIBRATING_UI;

        display.fillScreen(GC9A01A_BLACK);
        display.setCursor(40, 120);
        display.print("Calibrating");

        xTaskNotify(
            Earendil_Handles->Magnetometer_Handles.task_vMagnetometer_Calibrate, 0, eNoAction);
    }

    // =======================================================================================
    // MENU_UI UTILITIES
    // =======================================================================================

    // Menu structure
    struct MenuItem {
        const char* name; //name of the menu item
        MenuItem* parent; //pointer to the parent item
        MenuItem* children;//points to the child submenu array whenever i get to doing that
        uint8_t childCount;//this is used for the loop to keep track of array size that way when going into the array it knows how far to scroll
        void (*function)();  // pointer to a function that runs when selected
    };

    MenuItem calibrateMenu[] = {
        {   
            "Magnetometer", 
            NULL, 
            NULL, 
            0, 
            &request_Magnetometer_Calibrate
        }, 
        {
            "Temperature", 
            NULL, 
            NULL, 
            0, 
            NULL
        }, 
        {
            "Altimeter", 
            NULL, 
            NULL, 
            0, 
            NULL
        }, 
        {
            "Gyroscope", 
            NULL, 
            NULL, 
            0, 
            NULL
        }
    };

    MenuItem mainMenu[] = {
        {
            "Calibration", 
            NULL, 
            calibrateMenu, 
            4, 
            NULL
        }, 
        {
            "Info", 
            NULL, 
            NULL, 
            0, 
            NULL
        }, 
        {
            "Reset", 
            NULL, 
            NULL, 
            0, 
            NULL
        }, 
        {
            "Manual Ping", 
            NULL, 
            NULL, 
            0, 
            &request_sendPing_TX
        }, 
        {
            "Off", 
            NULL, 
            NULL, 
            0, 
            NULL
        }
    };

    MenuItem* currentMenu = mainMenu; //array pointer to the current menu to navigate parent and sub menus
    uint8_t selectedIndex = 0; //keeps track of the selected index to put the little indicator in the menu, 1 byte space saver. doubt we need more than 255 menu items
    uint8_t topIndex = 0; //keeps track of the scrolling window index
    uint8_t itemCount = sizeof(mainMenu) / sizeof(mainMenu[0]); //total array size in bytes / size of 1 element in bytes

    void setupMenu(){
        for (uint8_t i = 0; i < sizeof(calibrateMenu) / sizeof(calibrateMenu[0]); i++){
            calibrateMenu[i].parent = mainMenu;  // set parent pointer to the main menu. this will be used to go back
        }
    }

    void draw_MenuScreen_Static(){
        display.fillScreen(GC9A01A_BLACK);
        display.setCursor(X_MENU_OFFSET - 20, Y_MENU_OFFSET - 40);//sets the cursor to draw the text. x is from left to right byt a larger y is downward
        display.setTextColor(GC9A01A_WHITE);
        display.setTextSize(2);                 // scale the text to be bigger
        display.print("------ MENU ------");    // prints out menu icon
    }

    void draw_MenuScreen_Dynamic(){
        display.setCursor(X_MENU_OFFSET - 20, Y_MENU_OFFSET - 40);//sets the cursor to draw the text. x is from left to right byt a larger y is downward
        display.setTextColor(GC9A01A_WHITE);
        display.setTextSize(2);                 // scale the text to be bigger
        display.setCursor(X_MENU_OFFSET, Y_MENU_OFFSET); // back to its original spot
        
        uint8_t max_index = min(3, itemCount - topIndex);
        for (uint8_t i = 0; i < max_index; i++){
            int y = Y_MENU_OFFSET + (i * 40);
            uint8_t index = topIndex + i; // loops through and prints out each of the 3 indeces to be shown
            
            if (index == selectedIndex){ // makes a little indicator for what the index is looking at
                display.setTextColor(GC9A01A_GREEN);
                display.setCursor(X_MENU_OFFSET - 10, y);
                display.print("> ");
            } else {
                display.setTextColor(GC9A01A_WHITE);
            }
            display.setCursor(X_MENU_OFFSET, y);
            display.print(currentMenu[index].name);//prints out the name of the current option
        }
    }

    void moveUp(){
        if (selectedIndex > 0) selectedIndex --; //if theres space to move up, then move up
        if (selectedIndex < topIndex) topIndex --; //if theres stuff above the top index, that means it needs to be shown when it moves up
    }

    void moveDown(){
        if (selectedIndex < itemCount - 1) selectedIndex++; //if theres still items in the index to move through, then move
        if (selectedIndex >= topIndex + 3) topIndex++; //check if the selected index is larger than the top index by 3 to see if the top index should be moved down
    }

    //Working on getting the child submenu up and running. still working on it so dont select anything with a child node yet because it may break
    void selectItem(){
        MenuItem* selection = &currentMenu[selectedIndex];  // pointer to item inside of the current menu at the selected index
        if (selection -> children && selection -> childCount > 0){  // checks to see if the selection actually has a child menu to go to and has a childcount to ensure its indexed properly
            currentMenu = selection->children;  // array pointer to the current menu to navigate parent and sub menus
            selectedIndex = 0;                  // keeps track of the selected index to put the little indicator in the menu, 1 byte space saver. doubt we need more than 255 menu items
            topIndex = 0;                       // keeps track of the scrolling window index
            itemCount = selection->childCount;  // total array size
        } else if (selection->function) { // if theres no child menu, check if theres a function
            selection->function(); // call the function
        }
    }

    bool goBack() {
        if (currentMenu[0].parent){ // if theres a parent, then continue
            currentMenu = currentMenu[0].parent; // go back to parent menu array
            itemCount = sizeof(mainMenu)/sizeof(mainMenu[0]); // update the amount of items inside of the list
            selectedIndex = 0;  // start at the top
            topIndex = 0;
            return true;
        } else {
            return false;
        }
    }
    
    void menuControl(){
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        
        // BUTTON 1 PRESSED
        if (gpio_get(BUTTON1) == 0){
            if (active_ui == TESTING_UI){
                active_ui = MENU_UI;
            } else {
                active_ui = TESTING_UI;
            }
            while (gpio_get(BUTTON1) == 0) vTaskDelay(pdMS_TO_TICKS(5));
        }
        gpio_set_irq_enabled(BUTTON1, GPIO_IRQ_EDGE_FALL, true);

        // BACK BUTTON PRESSED
        if (gpio_get(BUTTON_BACK) == 0){
            if (!goBack()){
                active_ui = NAVIGATION_UI;
            }
            while (gpio_get(BUTTON_BACK) == 0) vTaskDelay(pdMS_TO_TICKS(5));
        }
        gpio_set_irq_enabled(BUTTON_BACK, GPIO_IRQ_EDGE_FALL, true);

        // SELECT BUTTON PRESSED
        if (gpio_get(BUTTON_SELECT) == 0){
            selectItem();
            while (gpio_get(BUTTON_SELECT) == 0) vTaskDelay(pdMS_TO_TICKS(5));
        }
        gpio_set_irq_enabled(BUTTON_SELECT, GPIO_IRQ_EDGE_FALL, true);

        // DOWN BUTTON PRESSED
        if (gpio_get(BUTTON_DOWN) == 0){
            moveDown();
            while (gpio_get(BUTTON_DOWN) == 0) vTaskDelay(pdMS_TO_TICKS(5));
        }
        gpio_set_irq_enabled(BUTTON_DOWN, GPIO_IRQ_EDGE_FALL, true);
        
        // UP BUTTON PRESSED
        if (gpio_get(BUTTON_UP) == 0){
            moveUp();
            while (gpio_get(BUTTON_UP) == 0) vTaskDelay(pdMS_TO_TICKS(5));
        }
        gpio_set_irq_enabled(BUTTON_UP, GPIO_IRQ_EDGE_FALL, true);
        
        vTaskDelay(pdMS_TO_TICKS(20));
    }

    // =======================================================================================
    // NAVIGATION_UI UTILITIES
    // =======================================================================================

    // --- STATIC UI ELEMENTS ----------------------------------------------------------------

    void draw_NavScreen_Static(){
        display.fillScreen(NAV_BACKGROUND_COLOR);
        draw_GuideRails();
    }

    void draw_GuideRails(){
        display.drawCircle(
            DISPLAY_CENTER_X, DISPLAY_CENTER_Y,
            GUIDE_RAIL_RADIUS_INNER, GUIDE_RAIL_COLOR
        );
        display.drawCircle(
            DISPLAY_CENTER_X, DISPLAY_CENTER_Y,
            GUIDE_RAIL_RADIUS_OUTER, GUIDE_RAIL_COLOR
        );
    }

    // --- DYNAMIC UI ELEMENTS ---------------------------------------------------------------

    void draw_NavScreen_Dynamic(){
        double heading_north_deg        = Earendil_Data->Magnetometer_Data.heading_deg;
        
        double handheld_latitude_rad    = Earendil_Data->GPS_Data.latitude_rad;
        double handheld_longitude_rad   = Earendil_Data->GPS_Data.longitude_rad;

        double node_latitude_rad        = 38.957472     * (M_PI / 180.0);
        double node_longitude_rad       = -95.253361    * (M_PI / 180.0);

        double bearing_node_deg = 0;
        getBearingToNode(
            bearing_node_deg,
            handheld_latitude_rad,
            handheld_longitude_rad,
            node_latitude_rad,
            node_longitude_rad
        );

        double distance_node_m = 0;
        getDistanceToNode(
            distance_node_m,
            handheld_latitude_rad,
            handheld_longitude_rad,
            node_latitude_rad,
            node_longitude_rad
        );
        
        clear_LastBearingToNode();
        clear_LastHeadingToNorth();
        clear_LastCardinalDirections();
        clear_DistanceToNode();

        draw_DistanceToNode(1488.123456789);
        draw_CardinalDirections(heading_north_deg);
        draw_HeadingToNorth(heading_north_deg);
        draw_BearingToNode(heading_north_deg, bearing_node_deg);    
        
        /*
        display.fillRect(DISPLAY_CENTER_X - 50, DISPLAY_CENTER_Y - 30, 120, 90, NAV_BACKGROUND_COLOR);

        display.setCursor(DISPLAY_CENTER_X - 50, DISPLAY_CENTER_Y - 30);
        display.setTextSize(3);
        display.setTextColor(GC9A01A_RED);
        display.print(heading_north_deg);

        display.setCursor(DISPLAY_CENTER_X - 50, DISPLAY_CENTER_Y - 30 + 30);
        display.setTextColor(GC9A01A_YELLOW);
        display.print(bearing_node_deg);
        */
    }

    void draw_HeadingToNorth(
        double heading_north_deg
    ){
        double  heading_track_radius_corr = HEADING_TRIANGLE_HEIGHT / 6.0;
        int16_t heading_N_center_X  = DISPLAY_CENTER_X + (HEADING_TRACK_RADIUS - heading_track_radius_corr) * cos( (heading_north_deg + 90.0) * (M_PI / 180.0));
        int16_t heading_N_center_Y  = DISPLAY_CENTER_Y - (HEADING_TRACK_RADIUS - heading_track_radius_corr) * sin( (heading_north_deg + 90.0) * (M_PI / 180.0));
        
        double heading_triangle_radius = HEADING_TRIANGLE_HEIGHT * (2.0 / 3.0);
        
        int16_t heading_N_vertex_X0 = heading_N_center_X + heading_triangle_radius * cos( (heading_north_deg + 0.0 + 90.0) * (M_PI / 180.0));
        int16_t heading_N_vertex_Y0 = heading_N_center_Y - heading_triangle_radius * sin( (heading_north_deg + 0.0 + 90.0) * (M_PI / 180.0));
        int16_t heading_N_vertex_X1 = heading_N_center_X + heading_triangle_radius * cos( (heading_north_deg + 120.0 + 90.0) * (M_PI / 180.0));
        int16_t heading_N_vertex_Y1 = heading_N_center_Y - heading_triangle_radius * sin( (heading_north_deg + 120.0 + 90.0) * (M_PI / 180.0));
        int16_t heading_N_vertex_X2 = heading_N_center_X + heading_triangle_radius * cos( (heading_north_deg + 240.0 + 90.0) * (M_PI / 180.0));
        int16_t heading_N_vertex_Y2 = heading_N_center_Y - heading_triangle_radius * sin( (heading_north_deg + 240.0 + 90.0) * (M_PI / 180.0));

        display.fillTriangle(
            heading_N_vertex_X0, 
            heading_N_vertex_Y0, 
            heading_N_vertex_X1, 
            heading_N_vertex_Y1,
            heading_N_vertex_X2, 
            heading_N_vertex_Y2, 
            HEADING_NORTH_COLOR
        );

        NavState.heading_N_vertex_X0 = heading_N_vertex_X0;
        NavState.heading_N_vertex_Y0 = heading_N_vertex_Y0;
        NavState.heading_N_vertex_X1 = heading_N_vertex_X1;
        NavState.heading_N_vertex_Y1 = heading_N_vertex_Y1;
        NavState.heading_N_vertex_X2 = heading_N_vertex_X2;
        NavState.heading_N_vertex_Y2 = heading_N_vertex_Y2;
    }

    void clear_LastHeadingToNorth(){
        if ( 
            (NavState.heading_N_vertex_X0 >= 0) &&
            (NavState.heading_N_vertex_Y0 >= 0) &&
            (NavState.heading_N_vertex_X1 >= 0) &&
            (NavState.heading_N_vertex_Y1 >= 0) &&
            (NavState.heading_N_vertex_X2 >= 0) &&
            (NavState.heading_N_vertex_Y2 >= 0)
        ){
            display.fillTriangle(
                NavState.heading_N_vertex_X0, 
                NavState.heading_N_vertex_Y0, 
                NavState.heading_N_vertex_X1, 
                NavState.heading_N_vertex_Y1,
                NavState.heading_N_vertex_X2, 
                NavState.heading_N_vertex_Y2, 
                NAV_BACKGROUND_COLOR
            );
        }
    }

    void draw_BearingToNode(
        double heading_north_deg,
        double bearing_node_deg
    ){
        double node_angle_deg   = heading_north_deg - bearing_node_deg;
        int16_t bearing_node_X  = DISPLAY_CENTER_X + BEARING_TRACK_RADIUS * cos( (node_angle_deg + 90.0) * (M_PI / 180.0));
        int16_t bearing_node_Y  = DISPLAY_CENTER_Y - BEARING_TRACK_RADIUS * sin( (node_angle_deg + 90.0) * (M_PI / 180.0));

        display.fillCircle(
            bearing_node_X, bearing_node_Y, 
            BEARING_CIRCLE_RADIUS, BEARING_NODE_COLOR
        );

        NavState.last_bearing_node_X = bearing_node_X;
        NavState.last_bearing_node_Y = bearing_node_Y;
    }

    void clear_LastBearingToNode(){
        if ( (NavState.last_bearing_node_X >= 0) && (NavState.last_bearing_node_Y >= 0) ){
            display.fillCircle(
                NavState.last_bearing_node_X, NavState.last_bearing_node_Y, 
                BEARING_CIRCLE_RADIUS, NAV_BACKGROUND_COLOR
            );
        }
    }

    void draw_CardinalDirections(
        double heading_north_deg
    ){
        // DRAW "N" (NORTH LABEL)
        double N_angle  = heading_north_deg;
        draw_CardinalLabel(
            N_angle,
            "N",
            CARDINAL_DIRS_TEXTSIZE_N,
            CARDINAL_DIRS_COLOR_N,
            NavState.last_cardinal_N_label_X,
            NavState.last_cardinal_N_label_Y,
            NavState.last_cardinal_N_label_width,
            NavState.last_cardinal_N_label_height
        );

        // DRAW "W" (WEST LABEL)
        double W_angle   = N_angle + 90.0;
        draw_CardinalLabel(
            W_angle,
            "W",
            CARDINAL_DIRS_TEXTSIZE_W,
            CARDINAL_DIRS_COLOR_W,
            NavState.last_cardinal_W_label_X,
            NavState.last_cardinal_W_label_Y,
            NavState.last_cardinal_W_label_width,
            NavState.last_cardinal_W_label_height
        );
        draw_CardinalNotch(
            W_angle,
            CARDINAL_DIRS_COLOR_W,
            NavState.last_cardinal_W_notch_X0,
            NavState.last_cardinal_W_notch_Y0,
            NavState.last_cardinal_W_notch_X1,
            NavState.last_cardinal_W_notch_Y1
        );

        // DRAW "S" (SOUTH LABEL)
        double S_angle  = W_angle + 90.0;
        draw_CardinalLabel(
            S_angle,
            "S",
            CARDINAL_DIRS_TEXTSIZE_S,
            CARDINAL_DIRS_COLOR_S,
            NavState.last_cardinal_S_label_X,
            NavState.last_cardinal_S_label_Y,
            NavState.last_cardinal_S_label_width,
            NavState.last_cardinal_S_label_height
        );
        draw_CardinalNotch(
            S_angle,
            CARDINAL_DIRS_COLOR_S,
            NavState.last_cardinal_S_notch_X0,
            NavState.last_cardinal_S_notch_Y0,
            NavState.last_cardinal_S_notch_X1,
            NavState.last_cardinal_S_notch_Y1
        );

        // DRAW "E" (EAST LABEL)
        double E_angle   = S_angle + 90.0;
        draw_CardinalLabel(
            E_angle,
            "E",
            CARDINAL_DIRS_TEXTSIZE_E,
            CARDINAL_DIRS_COLOR_E,
            NavState.last_cardinal_E_label_X,
            NavState.last_cardinal_E_label_Y,
            NavState.last_cardinal_E_label_width,
            NavState.last_cardinal_E_label_height
        );
        draw_CardinalNotch(
            E_angle,
            CARDINAL_DIRS_COLOR_E,
            NavState.last_cardinal_E_notch_X0,
            NavState.last_cardinal_E_notch_Y0,
            NavState.last_cardinal_E_notch_X1,
            NavState.last_cardinal_E_notch_Y1
        );

        // DRAW "NW" (NORTH-WEST LABEL)
        double NW_angle     = N_angle + 45.0;
        draw_CardinalLabel(
            NW_angle,
            "NW",
            CARDINAL_DIRS_TEXTSIZE_NW,
            CARDINAL_DIRS_COLOR_NW,
            NavState.last_cardinal_NW_label_X,
            NavState.last_cardinal_NW_label_Y,
            NavState.last_cardinal_NW_label_width,
            NavState.last_cardinal_NW_label_height
        );
        draw_CardinalNotch(
            NW_angle,
            CARDINAL_DIRS_COLOR_NW,
            NavState.last_cardinal_NW_notch_X0,
            NavState.last_cardinal_NW_notch_Y0,
            NavState.last_cardinal_NW_notch_X1,
            NavState.last_cardinal_NW_notch_Y1
        );

        // DRAW "SW" (SOUTH-WEST LABEL)
        double SW_angle     = NW_angle + 90.0;
        draw_CardinalLabel(
            SW_angle,
            "SW",
            CARDINAL_DIRS_TEXTSIZE_SW,
            CARDINAL_DIRS_COLOR_SW,
            NavState.last_cardinal_SW_label_X,
            NavState.last_cardinal_SW_label_Y,
            NavState.last_cardinal_SW_label_width,
            NavState.last_cardinal_SW_label_height
        );
        draw_CardinalNotch(
            SW_angle,
            CARDINAL_DIRS_COLOR_SW,
            NavState.last_cardinal_SW_notch_X0,
            NavState.last_cardinal_SW_notch_Y0,
            NavState.last_cardinal_SW_notch_X1,
            NavState.last_cardinal_SW_notch_Y1
        );

        // DRAW "SE" (SOUTH-EAST LABEL)
        double SE_angle     = SW_angle + 90.0;
        draw_CardinalLabel(
            SE_angle,
            "SE",
            CARDINAL_DIRS_TEXTSIZE_SE,
            CARDINAL_DIRS_COLOR_SE,
            NavState.last_cardinal_SE_label_X,
            NavState.last_cardinal_SE_label_Y,
            NavState.last_cardinal_SE_label_width,
            NavState.last_cardinal_SE_label_height
        );
        draw_CardinalNotch(
            SE_angle,
            CARDINAL_DIRS_COLOR_SE,
            NavState.last_cardinal_SE_notch_X0,
            NavState.last_cardinal_SE_notch_Y0,
            NavState.last_cardinal_SE_notch_X1,
            NavState.last_cardinal_SE_notch_Y1
        );

        // DRAW "NE" (NORTH-EAST LABEL)
        double NE_angle     = SE_angle + 90.0;
        draw_CardinalLabel(
            NE_angle,
            "NE",
            CARDINAL_DIRS_TEXTSIZE_NE,
            CARDINAL_DIRS_COLOR_NE,
            NavState.last_cardinal_NE_label_X,
            NavState.last_cardinal_NE_label_Y,
            NavState.last_cardinal_NE_label_width,
            NavState.last_cardinal_NE_label_height
        );
        draw_CardinalNotch(
            NE_angle,
            CARDINAL_DIRS_COLOR_SE,
            NavState.last_cardinal_NE_notch_X0,
            NavState.last_cardinal_NE_notch_Y0,
            NavState.last_cardinal_NE_notch_X1,
            NavState.last_cardinal_NE_notch_Y1
        );
    }

    void draw_CardinalLabel(
        double      cardinal_angle,
        const char* cardinal_label_text,
        uint8_t     cardinal_text_size,
        uint16_t    cardinal_color,
        int16_t&    last_cardinal_label_X,
        int16_t&    last_cardinal_label_Y,
        uint16_t&   last_cardinal_label_width,
        uint16_t&   last_cardinal_label_height
    ){
        int16_t cardinal_label_X_raw    = DISPLAY_CENTER_X + CARDINAL_DIRS_RADIUS * cos( (cardinal_angle + 90) * (M_PI / 180.0));
        int16_t cardinal_label_Y_raw    = DISPLAY_CENTER_Y - CARDINAL_DIRS_RADIUS * sin( (cardinal_angle + 90) * (M_PI / 180.0));

        int16_t cardinal_label_corner_X = 0;
        int16_t cardinal_label_corner_Y = 0;
        uint16_t cardinal_label_width   = 0;
        uint16_t cardinal_label_height  = 0;

        display.setTextSize(cardinal_text_size);
        display.getTextBounds(
            cardinal_label_text,
            cardinal_label_X_raw,
            cardinal_label_Y_raw,
            &cardinal_label_corner_X,
            &cardinal_label_corner_Y,
            &cardinal_label_width,
            &cardinal_label_height
        );
        
        int16_t cardinal_label_X  = cardinal_label_X_raw - (cardinal_label_corner_X - cardinal_label_X_raw) - (cardinal_label_width / 2)  + 2;
        int16_t cardinal_label_Y  = cardinal_label_Y_raw - (cardinal_label_corner_Y - cardinal_label_Y_raw) - (cardinal_label_height / 2) + 2;

        display.setCursor(cardinal_label_X, cardinal_label_Y);
        display.setTextColor(cardinal_color);
        display.print(cardinal_label_text);

        last_cardinal_label_X       = cardinal_label_X;
        last_cardinal_label_Y       = cardinal_label_Y;
        last_cardinal_label_width   = cardinal_label_width;
        last_cardinal_label_height  = cardinal_label_height;
    }

    void draw_CardinalNotch(
        double      cardinal_angle,
        uint16_t    cardinal_color,
        int16_t&    last_cardinal_notch_X0,
        int16_t&    last_cardinal_notch_Y0,
        int16_t&    last_cardinal_notch_X1,
        int16_t&    last_cardinal_notch_Y1
    ){
        int16_t cardinal_notch_X0   = DISPLAY_CENTER_X + (GUIDE_RAIL_RADIUS_INNER + 5) * cos( (cardinal_angle + 90) * (M_PI / 180.0));
        int16_t cardinal_notch_Y0   = DISPLAY_CENTER_Y - (GUIDE_RAIL_RADIUS_INNER + 5) * sin( (cardinal_angle + 90) * (M_PI / 180.0));
        int16_t cardinal_notch_X1   = DISPLAY_CENTER_X + (GUIDE_RAIL_RADIUS_OUTER - 5) * cos( (cardinal_angle + 90) * (M_PI / 180.0));
        int16_t cardinal_notch_Y1   = DISPLAY_CENTER_Y - (GUIDE_RAIL_RADIUS_OUTER - 5) * sin( (cardinal_angle + 90) * (M_PI / 180.0));

        display.drawLine(
            cardinal_notch_X0,
            cardinal_notch_Y0,
            cardinal_notch_X1,
            cardinal_notch_Y1,
            cardinal_color
        );

        last_cardinal_notch_X0 = cardinal_notch_X0;
        last_cardinal_notch_Y0 = cardinal_notch_Y0;
        last_cardinal_notch_X1 = cardinal_notch_X1;
        last_cardinal_notch_Y1 = cardinal_notch_Y1;
    }

    void clear_LastCardinalDirections(){
        if (    
            (NavState.last_cardinal_N_label_X >= 0)       && 
            (NavState.last_cardinal_N_label_Y >= 0)       &&
            (NavState.last_cardinal_N_label_width >= 0)   &&
            (NavState.last_cardinal_N_label_height >= 0)
        ){
            display.fillRect(
                NavState.last_cardinal_N_label_X,
                NavState.last_cardinal_N_label_Y,
                NavState.last_cardinal_N_label_width,
                NavState.last_cardinal_N_label_height,
                NAV_BACKGROUND_COLOR
            );
        }
        if (    
            (NavState.last_cardinal_W_label_X >= 0)         && 
            (NavState.last_cardinal_W_label_Y >= 0)         &&
            (NavState.last_cardinal_W_label_width >= 0)     &&
            (NavState.last_cardinal_W_label_height >= 0)    &&
            (NavState.last_cardinal_W_notch_X0 >= 0)        &&
            (NavState.last_cardinal_W_notch_Y0 >= 0)        &&
            (NavState.last_cardinal_W_notch_X1 >= 0)        &&
            (NavState.last_cardinal_W_notch_Y1 >= 0)        
        ){
            display.fillRect(
                NavState.last_cardinal_W_label_X,
                NavState.last_cardinal_W_label_Y,
                NavState.last_cardinal_W_label_width,
                NavState.last_cardinal_W_label_height,
                NAV_BACKGROUND_COLOR
            );
            display.drawLine(
                NavState.last_cardinal_W_notch_X0,
                NavState.last_cardinal_W_notch_Y0,
                NavState.last_cardinal_W_notch_X1,
                NavState.last_cardinal_W_notch_Y1,
                NAV_BACKGROUND_COLOR
            );
        }
        if (    
            (NavState.last_cardinal_S_label_X >= 0)         && 
            (NavState.last_cardinal_S_label_Y >= 0)         &&
            (NavState.last_cardinal_S_label_width >= 0)     &&
            (NavState.last_cardinal_S_label_height >= 0)    &&
            (NavState.last_cardinal_S_notch_X0 >= 0)        &&
            (NavState.last_cardinal_S_notch_Y0 >= 0)        &&
            (NavState.last_cardinal_S_notch_X1 >= 0)        &&
            (NavState.last_cardinal_S_notch_Y1 >= 0) 
        ){
            display.fillRect(
                NavState.last_cardinal_S_label_X,
                NavState.last_cardinal_S_label_Y,
                NavState.last_cardinal_S_label_width,
                NavState.last_cardinal_S_label_height,
                NAV_BACKGROUND_COLOR
            );
            display.drawLine(
                NavState.last_cardinal_S_notch_X0,
                NavState.last_cardinal_S_notch_Y0,
                NavState.last_cardinal_S_notch_X1,
                NavState.last_cardinal_S_notch_Y1,
                NAV_BACKGROUND_COLOR
            );
        }
        if (    
            (NavState.last_cardinal_E_label_X >= 0)         && 
            (NavState.last_cardinal_E_label_Y >= 0)         &&
            (NavState.last_cardinal_E_label_width >= 0)     &&
            (NavState.last_cardinal_E_label_height >= 0)    &&
            (NavState.last_cardinal_E_notch_X0 >= 0)        &&
            (NavState.last_cardinal_E_notch_Y0 >= 0)        &&
            (NavState.last_cardinal_E_notch_X1 >= 0)        &&
            (NavState.last_cardinal_E_notch_Y1 >= 0) 
        ){
            display.fillRect(
                NavState.last_cardinal_E_label_X,
                NavState.last_cardinal_E_label_Y,
                NavState.last_cardinal_E_label_width,
                NavState.last_cardinal_E_label_height,
                NAV_BACKGROUND_COLOR
            );
            display.drawLine(
                NavState.last_cardinal_E_notch_X0,
                NavState.last_cardinal_E_notch_Y0,
                NavState.last_cardinal_E_notch_X1,
                NavState.last_cardinal_E_notch_Y1,
                NAV_BACKGROUND_COLOR
            );
        }
        if (    
            (NavState.last_cardinal_NW_label_X >= 0)        && 
            (NavState.last_cardinal_NW_label_Y >= 0)        &&
            (NavState.last_cardinal_NW_label_width >= 0)    &&
            (NavState.last_cardinal_NW_label_height >= 0)   &&
            (NavState.last_cardinal_NW_notch_X0 >= 0)       &&
            (NavState.last_cardinal_NW_notch_Y0 >= 0)       &&
            (NavState.last_cardinal_NW_notch_X1 >= 0)       &&
            (NavState.last_cardinal_NW_notch_Y1 >= 0) 
        ){
            display.fillRect(
                NavState.last_cardinal_NW_label_X,
                NavState.last_cardinal_NW_label_Y,
                NavState.last_cardinal_NW_label_width,
                NavState.last_cardinal_NW_label_height,
                NAV_BACKGROUND_COLOR
            );
            display.drawLine(
                NavState.last_cardinal_NW_notch_X0,
                NavState.last_cardinal_NW_notch_Y0,
                NavState.last_cardinal_NW_notch_X1,
                NavState.last_cardinal_NW_notch_Y1,
                NAV_BACKGROUND_COLOR
            );
        }
        if (    
            (NavState.last_cardinal_SW_label_X >= 0)        && 
            (NavState.last_cardinal_SW_label_Y >= 0)        &&
            (NavState.last_cardinal_SW_label_width >= 0)    &&
            (NavState.last_cardinal_SW_label_height >= 0)   &&
            (NavState.last_cardinal_SW_notch_X0 >= 0)       &&
            (NavState.last_cardinal_SW_notch_Y0 >= 0)       &&
            (NavState.last_cardinal_SW_notch_X1 >= 0)       &&
            (NavState.last_cardinal_SW_notch_Y1 >= 0) 
        ){
            display.fillRect(
                NavState.last_cardinal_SW_label_X,
                NavState.last_cardinal_SW_label_Y,
                NavState.last_cardinal_SW_label_width,
                NavState.last_cardinal_SW_label_height,
                NAV_BACKGROUND_COLOR
            );
            display.drawLine(
                NavState.last_cardinal_SW_notch_X0,
                NavState.last_cardinal_SW_notch_Y0,
                NavState.last_cardinal_SW_notch_X1,
                NavState.last_cardinal_SW_notch_Y1,
                NAV_BACKGROUND_COLOR
            );
        }
        if (    
            (NavState.last_cardinal_SE_label_X >= 0)        && 
            (NavState.last_cardinal_SE_label_Y >= 0)        &&
            (NavState.last_cardinal_SE_label_width >= 0)    &&
            (NavState.last_cardinal_SE_label_height >= 0)   &&
            (NavState.last_cardinal_SE_notch_X0 >= 0)       &&
            (NavState.last_cardinal_SE_notch_Y0 >= 0)       &&
            (NavState.last_cardinal_SE_notch_X1 >= 0)       &&
            (NavState.last_cardinal_SE_notch_Y1 >= 0) 
        ){
            display.fillRect(
                NavState.last_cardinal_SE_label_X,
                NavState.last_cardinal_SE_label_Y,
                NavState.last_cardinal_SE_label_width,
                NavState.last_cardinal_SE_label_height,
                NAV_BACKGROUND_COLOR
            );
            display.drawLine(
                NavState.last_cardinal_SE_notch_X0,
                NavState.last_cardinal_SE_notch_Y0,
                NavState.last_cardinal_SE_notch_X1,
                NavState.last_cardinal_SE_notch_Y1,
                NAV_BACKGROUND_COLOR
            );
        }
        if (    
            (NavState.last_cardinal_NE_label_X >= 0)        && 
            (NavState.last_cardinal_NE_label_Y >= 0)        &&
            (NavState.last_cardinal_NE_label_width >= 0)    &&
            (NavState.last_cardinal_NE_label_height >= 0)   &&
            (NavState.last_cardinal_NE_notch_X0 >= 0)       &&
            (NavState.last_cardinal_NE_notch_Y0 >= 0)       &&
            (NavState.last_cardinal_NE_notch_X1 >= 0)       &&
            (NavState.last_cardinal_NE_notch_Y1 >= 0) 
            
        ){
            display.fillRect(
                NavState.last_cardinal_NE_label_X,
                NavState.last_cardinal_NE_label_Y,
                NavState.last_cardinal_NE_label_width,
                NavState.last_cardinal_NE_label_height,
                NAV_BACKGROUND_COLOR
            );
            display.drawLine(
                NavState.last_cardinal_NE_notch_X0,
                NavState.last_cardinal_NE_notch_Y0,
                NavState.last_cardinal_NE_notch_X1,
                NavState.last_cardinal_NE_notch_Y1,
                NAV_BACKGROUND_COLOR
            );
        }
    }
    
    void draw_DistanceToNode(
        double distance_node_m
    ){
        if (distance_node_m < DISTANCE_MAX_DISPL_VALUE){
            char distance_str_buf[8];
            snprintf(distance_str_buf, sizeof(distance_str_buf), "%.2f", distance_node_m);
            const char* distance_str = distance_str_buf;

            int16_t distance_label_X_raw    = DISPLAY_CENTER_X;
            int16_t distance_label_Y_raw    = DISPLAY_CENTER_Y;

            int16_t distance_label_corner_X = 0;
            int16_t distance_label_corner_Y = 0;
            uint16_t distance_label_width   = 0;
            uint16_t distance_label_height  = 0;

            display.setTextSize(DISTANCE_TEXT_SIZE);
            display.getTextBounds(
                distance_str,
                distance_label_X_raw,
                distance_label_Y_raw,
                &distance_label_corner_X,
                &distance_label_corner_Y,
                &distance_label_width,
                &distance_label_height
            );
            
            int16_t distance_label_X    = distance_label_X_raw - (distance_label_width / 4.0);
            int16_t distance_label_Y    = distance_label_Y_raw - (distance_label_corner_Y - distance_label_Y_raw) - (distance_label_height / 2.0) + 2;

            display.setCursor(distance_label_X, distance_label_Y_raw);
            display.setTextColor(DISTANCE_TEXT_COLOR);
            display.print(distance_str);

            NavState.last_distance_label_X       = distance_label_X;
            NavState.last_distance_label_Y       = distance_label_Y;
            NavState.last_distance_label_width   = distance_label_width;
            NavState.last_distance_label_height  = distance_label_height;
        } else {
            ; // More than DISTANCE_MAX_DISPL_VALUE away case.
        }
    }

    void clear_DistanceToNode(){

    }

    // --- UX CONTROLS -----------------------------------------------------------------------

    void controlNav(){
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        
        if (gpio_get(BUTTON_BACK) == 0){
            active_ui = MENU_UI;
            while (gpio_get(BUTTON_BACK) == 0) vTaskDelay(pdMS_TO_TICKS(5));
        }
        gpio_set_irq_enabled(BUTTON_BACK, GPIO_IRQ_EDGE_FALL, true);

    }

    // =======================================================================================
    // TESTING UTILITIES
    // =======================================================================================

    void getBearingToNode(
        double& bearing_to_node_deg,
        double  handheld_latitude_rad,
        double  handheld_longitude_rad,
        double  node_latitude_rad,
        double  node_longitude_rad
    ){
        double handheld_X   = cos(handheld_latitude_rad) * cos(handheld_longitude_rad);
        double handheld_Y   = cos(handheld_latitude_rad) * sin(handheld_longitude_rad);
        double handheld_Z   = sin(handheld_latitude_rad);
        
        double node_X       = cos(node_latitude_rad) * cos(node_longitude_rad);
        double node_Y       = cos(node_latitude_rad) * sin(node_longitude_rad);
        double node_Z       = sin(node_latitude_rad);

        double Hx_sqr   = handheld_X * handheld_X;
        double Hy_sqr   = handheld_Y * handheld_Y;
        double Hz_sqr   = handheld_Z * handheld_Z;
        double HxNx     = handheld_X * node_X;
        double HyNy     = handheld_Y * node_Y;
        double HzNz     = handheld_Z * node_Z;

        double mag_north_vec_X = (-1) * handheld_X * handheld_Z;
        double mag_north_vec_Y = (-1) * handheld_Y * handheld_Z;
        double mag_north_vec_Z = (Hx_sqr + Hy_sqr);

        double bearing_vec_X = node_X * (Hy_sqr + Hz_sqr) - handheld_X * (HyNy + HzNz); 
        double bearing_vec_Y = node_Y * (Hx_sqr + Hz_sqr) - handheld_Y * (HxNx + HzNz);
        double bearing_vec_Z = node_Z * (Hx_sqr + Hy_sqr) - handheld_Z * (HxNx + HyNy);
        
        double B_dot_M = mag_north_vec_X * bearing_vec_X + mag_north_vec_Y * bearing_vec_Y + mag_north_vec_Z * bearing_vec_Z;
        
        double B_cross_M_X = (bearing_vec_Y * mag_north_vec_Z - bearing_vec_Z * mag_north_vec_Y);
        double B_cross_M_Y = (bearing_vec_Z * mag_north_vec_X - bearing_vec_X * mag_north_vec_Z);
        double B_cross_M_Z = (bearing_vec_X * mag_north_vec_Y - bearing_vec_Y * mag_north_vec_X);
        
        double BxM_dot_H = B_cross_M_X * handheld_X + B_cross_M_Y * handheld_Y + B_cross_M_Z * handheld_Z;
        
        bearing_to_node_deg = atan2(BxM_dot_H, B_dot_M) * (180.0 / M_PI);
    }

    void getDistanceToNode(
        double& distance_node_m,
        double  handheld_latitude_rad,
        double  handheld_longitude_rad,
        double  node_latitude_rad,
        double  node_longitude_rad
    ){
        constexpr double EARTH_RADIUS = 6371008.8;  // In meters.

        double handheld_X   = cos(handheld_latitude_rad) * cos(handheld_longitude_rad);
        double handheld_Y   = cos(handheld_latitude_rad) * sin(handheld_longitude_rad);
        double handheld_Z   = sin(handheld_latitude_rad);
        
        double node_X       = cos(node_latitude_rad) * cos(node_longitude_rad);
        double node_Y       = cos(node_latitude_rad) * sin(node_longitude_rad);
        double node_Z       = sin(node_latitude_rad);

        double H_dot_N = handheld_X * node_X + handheld_Y * node_Y + handheld_Z * node_Z;

        double H_cross_N_X      = (handheld_Y * node_Z - handheld_Z * node_Y);
        double H_cross_N_Y      = (handheld_Z * node_X - handheld_X * node_Z);
        double H_cross_N_Z      = (handheld_X * node_Y - handheld_Y * node_X);
        double mag_H_cross_N    = sqrt(
            H_cross_N_X * H_cross_N_X + 
            H_cross_N_Y * H_cross_N_Y +
            H_cross_N_Z * H_cross_N_Z
        );
        
        double central_angle = atan2(mag_H_cross_N, H_dot_N);
        distance_node_m = EARTH_RADIUS * central_angle;
    }

    void drawTesting(){
        uint8_t init_y = 40;
        uint8_t x = 40;
        uint8_t y = init_y;
        
        double handheld_latitude_deg    = Earendil_Data->GPS_Data.latitude_deg;
        double handheld_longitude_deg   = Earendil_Data->GPS_Data.longitude_deg;
        double node_latitude_deg        = Earendil_Data->Radio_Data.rx_latitude_deg;
        double node_longitude_deg       = Earendil_Data->Radio_Data.rx_longitude_deg;

        display.fillRect(x, init_y, 180, 150, GC9A01A_BLACK);

        display.setTextColor(GC9A01A_WHITE);
        display.setTextSize(3);
        
        display.setCursor(x, y);
        display.print(handheld_latitude_deg, 6);
        y += 30;
        display.setCursor(x, y);
        display.print(handheld_longitude_deg, 6);

        y += 30;
        display.setCursor(x, y);
        display.print(node_latitude_deg, 6);
        y += 30;
        display.setCursor(x, y);
        display.print(node_longitude_deg, 6);
    }

} 