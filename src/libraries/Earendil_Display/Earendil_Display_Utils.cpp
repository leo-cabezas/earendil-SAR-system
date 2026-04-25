 #include <Earendil_Display.h>   // ATTENTION: Add all library dependencies inside this header.

namespace Earendil_Display {
    
    Adafruit_GC9A01A display = Adafruit_GC9A01A(TFT_CS, TFT_DC, TFT_RST);

    ACTIVE_UI active_ui;
    
    void setup(){
        setupMenuButtons();
        setupDisplay();
    }

    void setupDisplay(){
        display.begin();
        display.setRotation(0);
        display.fillScreen(GC9A01A_RED);
        display.setCursor(120, 120);
        display.setTextColor(GC9A01A_WHITE);

        active_ui = TESTING_UI;
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

    void request_Magnetometer_Calibrate()
    {
        active_ui = CALIBRATING_UI;

        display.fillScreen(GC9A01A_BLACK);
        display.setCursor(40, 120);
        display.print("Calibrating");

        xTaskNotify(
            Earendil_Handles->Magnetometer_Handles.task_vMagnetometer_Calibrate,0,eNoAction);
    }

    // =================================MENU STUFF==============================================================
    // Menu structure
    struct MenuItem {
        const char* name; //name of the menu item
        MenuItem* parent; //pointer to the parent item
        MenuItem* children;//points to the child submenu array whenever i get to doing that
        uint8_t childCount;//this is used for the loop to keep track of array size that way when going into the array it knows how far to scroll
        void (*function)();  // pointer to a function that runs when selected
    };

    MenuItem calibrateMenu[] = {
        {"Magnetometer", NULL, NULL, 0, &request_Magnetometer_Calibrate}, 
        {"Temperature", NULL, NULL, 0, NULL}, 
        {"Altimeter", NULL, NULL, 0, NULL}, 
        {"Gyroscope", NULL, NULL, 0, NULL}
    };

    // Array of menu items
    MenuItem mainMenu[] = {
        // {"Calibrate Magnetometer", NULL, NULL, 0, &request_Magnetometer_Calibrate},
        {"Calibration", NULL, calibrateMenu, 4, NULL}, 
        {"Info", NULL, NULL, 0, NULL}, 
        {"Reset", NULL, NULL, 0, NULL}, 
        {"Manual Ping", NULL, NULL, 0, &request_sendPing_TX}, 
        {"Off", NULL, NULL, 0, NULL}
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

    // Placeholder for now. everythigns in the serial monitor while I figure out the scrolling part. if I cant figure it out i'll go back to the original idea i had
    // yippee i figured out the scrolling part. pain in the ass, i needed a top index tracker
    void drawMenu(){
        display.fillScreen(GC9A01A_BLACK);
        display.setCursor(X_MENU_OFFSET-20, Y_MENU_OFFSET-40);//sets the cursor to draw the text. x is from left to right byt a larger y is downward
        display.setTextColor(GC9A01A_WHITE);
        display.setTextSize(2);                 // scale the text to be bigger
        display.print("------ MENU ------");    // prints out menu icon
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
        } else {
            Serial.println("Placeholder so stuff doesnt break");
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

    //=================================COMPASS UI STUFF==============================================================
    
    void drawMagneticNorth(){
        float heading = Earendil_Data->Magnetometer_Data.heading + 90;   // Need to protect this with a mutex !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        float x = DISPLAY_CENTER_X + 90.0 * cos(heading * (M_PI / 180.0));    // 120 is the origin, x uses sine to account for 90 degrees of rotation to get north. 
        float y = DISPLAY_CENTER_Y - 90.0 * sin(heading * (M_PI / 180.0));    // y coordinates face downward, so -cosine is used to account for downward face of the y axis and the 90 degree flip
        display.fillCircle(x, y, 5, GC9A01A_RED);//creates a circle on the edge of the screen to show off the angle
        
        display.setCursor(DISPLAY_CENTER_X - 30, DISPLAY_CENTER_Y);
        display.setTextColor(GC9A01A_YELLOW);
        display.setTextSize(3);
        display.print(heading - 90);
    }
    
    void displayNav(){
        display.fillScreen(GC9A01A_BLACK);  // Instead redraw the annulus where the indicator notches sit
        drawMagneticNorth();
        // drawDistance();
        // drawDirection();
    }

    struct handheld {
        double my_lat; //this will pull the lattitude from the GPS module
        double my_long; //this will pull the longitude from the GPS module

        double my_angle; //stores the angle from north from the magnetometer
        double distance_to_node;//distance from the handheld to the node
        double angle_from_north; //gets the relative angle from north so the user can spin around and still get an accurate direction
        double angle_to_node;//angle from the handheld face to the node
    };

    struct node {
        double node_lat;//pulls the lattidude from the gps
        double node_long;//same thing here
    };

    handheld my_handheld {0.6, 6.2, 0.0, 0.0, 0.0, 0.0};//hardcoded handheld data structure
    node node_1 {0.09, 0.0005}; //Hardcoded node instance structure to play around with

    GFXcanvas16 canvas(240, 240);
    int ui_state = 0; //0 for the compass, 1 for the menu
    bool lastState = HIGH; //checks last state of the button

    void drawDistance(){
        display.setCursor(75, 110);//sets the cursor to draw the text. x is from left to right byt a larger y is downward
        display.setTextColor(GC9A01A_WHITE);
        display.setTextSize(3);//scale the text to be bigger
        display.print(my_handheld.distance_to_node, 2);//prints out the distance to 2 decimal places
    }

    void drawDirection(){
        double bearing_to_node_deg;
        // getBearingToNode(bearing_to_node_deg);
        double relative_ang = my_handheld.angle_from_north + bearing_to_node_deg;
        if (relative_ang < 0) relative_ang += 360;

        double x = 120 + 105*sin(relative_ang*3.1415/180);//120 is the origin, x uses sine to account for 90 degrees of rotation to get north. 
        double y = 120 - 105*cos(relative_ang*3.1415/180);//y coordinates face downward, so -cosine is used to account for downward face of the y axis and the 90 degree flip
        display.fillCircle(x, y, 5, GC9A01A_WHITE);//creates a circle on the edge of the screen to show off the angle
    }

    void drawNotch(){
        display.fillCircle(120, 120, 115, OUTLINE_BLU);
        display.fillCircle(120, 120, 109, GC9A01A_BLACK);
        display.fillCircle(120, 120, 110, 0x1188);
        display.fillCircle(120, 120, 100, OUTLINE_BLU);
        display.fillCircle(120, 120, 95, GC9A01A_BLACK);
    }

    void controlNav(){
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        
        if (gpio_get(BUTTON_BACK) == 0){
            active_ui = MENU_UI;
            while (gpio_get(BUTTON_BACK) == 0) vTaskDelay(pdMS_TO_TICKS(5));
        }
        gpio_set_irq_enabled(BUTTON_BACK, GPIO_IRQ_EDGE_FALL, true);
    }

    // =================================== TESTING STUFF =============================================

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
        
        double normal_X = handheld_X;
        double normal_Y = handheld_Y;
        double normal_Z = handheld_Z;
        
        double BxM_dot_N = B_cross_M_X * normal_X + B_cross_M_Y * normal_Y + B_cross_M_Z * normal_Z;
        
        bearing_to_node_deg = atan2(BxM_dot_N, B_dot_M) * (180.0 / M_PI);
    }

    void drawTesting(){
        uint8_t init_y = 40;
        uint8_t x = 40;
        uint8_t y = init_y;
        
        double handheld_latitude_rad    = Earendil_Data->GPS_Data.latitude_rad;
        double handheld_longitude_rad   = Earendil_Data->GPS_Data.longitude_rad;
        double node_latitude_rad        = Earendil_Data->Radio_Data.rx_latitude_rad;
        double node_longitude_rad       = Earendil_Data->Radio_Data.rx_longitude_rad;

        double bearing_to_node_deg = 0;
        getBearingToNode(
            bearing_to_node_deg,
            handheld_latitude_rad,
            handheld_longitude_rad,
            node_latitude_rad,
            node_longitude_rad
        );

        display.fillRect(x, init_y, 180, 150, GC9A01A_BLACK);

        display.setTextColor(GC9A01A_WHITE);
        display.setTextSize(3);
        
        display.setCursor(x, y);
        display.print(handheld_latitude_rad, 6);
        y += 30;
        display.setCursor(x, y);
        display.print(handheld_longitude_rad, 6);

        y += 30;
        display.setCursor(x, y);
        display.print(node_latitude_rad, 6);
        y += 30;
        display.setCursor(x, y);
        display.print(node_longitude_rad, 6);

        y+= 30;
        display.setCursor(x, y);
        display.print(bearing_to_node_deg, 6);
    }

} 