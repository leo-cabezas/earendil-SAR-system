#include <Earendil_Display.h> // ATTENTION: Add all library dependencies to this header.

Adafruit_GC9A01A tft = Adafruit_GC9A01A(TFT_CS, TFT_DC, TFT_RST);
//Adafruit_GC9A01A tft = Adafruit_GC9A01A(TFT_CS, TFT_DC, TFT_MOSI,TFT_SCLK, TFT_RST, TFT_MISO);//initialization for the adafruit screen structure

//=================================MENU STUFF==============================================================
//Menu structure
struct MenuItem
{
  const char* name; //name of the menu item
  MenuItem* parent; //pointer to the parent item
  MenuItem* children;//points to the child submenu array whenever i get to doing that
  uint8_t childCount;//this is used for the loop to keep track of array size that way when going into the array it knows how far to scroll
  void (*function)();  // pointer to a function that runs when selected
};

MenuItem calibrateMenu[] =
{
  {"Magnetometer", NULL, NULL, 0, NULL}, {"Temperature", NULL, NULL, 0, NULL}, {"Altimeter", NULL, NULL, 0, NULL}, {"Gyroscope", NULL, NULL, 0, NULL}
};
//Array of menu items
MenuItem mainMenu[] =
{
  {"Calibration", NULL, calibrateMenu, 4, NULL}, {"Info", NULL, NULL, 0, NULL}, {"Reset", NULL, NULL, 0, NULL}, {"Manual Ping", NULL, NULL, 0, NULL}, {"Off", NULL, NULL, 0, NULL}
};

MenuItem* currentMenu = mainMenu; //array pointer to the current menu to navigate parent and sub menus
uint8_t selectedIndex = 0; //keeps track of the selected index to put the little indicator in the menu, 1 byte space saver. doubt we need more than 255 menu items
uint8_t topIndex = 0; //keeps track of the scrolling window index
uint8_t itemCount = sizeof(mainMenu)/sizeof(mainMenu[0]); //total array size in bytes / size of 1 element in bytes

//Placeholder for now. everythigns in the serial monitor while I figure out the scrolling part. if I cant figure it out i'll go back to the original idea i had
//yippee i figured out the scrolling part. pain in the ass, i needed a top index tracker
void drawMenu()
{
  tft.fillScreen(GC9A01A_BLACK);
  tft.setCursor(X_MENU_OFFSET-20, Y_MENU_OFFSET-40);//sets the cursor to draw the text. x is from left to right byt a larger y is downward
  tft.setTextColor(GC9A01A_WHITE);
  tft.setTextSize(2);//scale the text to be bigger
  tft.print("------ MENU ------");//prints out menu icon
  tft.setCursor(X_MENU_OFFSET, Y_MENU_OFFSET); //back to its original spot
  uint8_t max_index = min(3, itemCount - topIndex);
  for (uint8_t i = 0; i < max_index; i++) //loops through the 3 items
  {
    int y = Y_MENU_OFFSET + (i*40);
    uint8_t index = topIndex + i; //loops through and prints out each of the 3 indeces to be shown
    if (index == selectedIndex) //makes a little indicator for what the index is looking at
    {
      tft.setTextColor(GC9A01A_GREEN);
      tft.setCursor(X_MENU_OFFSET - 10, y);
      tft.print("> ");
    } 
    else
    {
      tft.setTextColor(GC9A01A_WHITE);
    }
    tft.setCursor(X_MENU_OFFSET, y);
    tft.print(currentMenu[index].name);//prints out the name of the current option
  }
}

//Move the selected menu item up
void moveUp()
{
  if (selectedIndex > 0) selectedIndex --; //if theres space to move up, then move up
  if (selectedIndex < topIndex) topIndex --; //if theres stuff above the top index, that means it needs to be shown when it moves up
}

void moveDown()
{
  if (selectedIndex < itemCount - 1) selectedIndex++; //if theres still items in the index to move through, then move
  if (selectedIndex >= topIndex + 3) topIndex++; //check if the selected index is larger than the top index by 3 to see if the top index should be moved down
}

//Working on getting the child submenu up and running. still working on it so dont select anything with a child node yet because it may break
void selectItem()
{
  MenuItem* selection = &currentMenu[selectedIndex];//pointer to item inside of the current menu at the selected index
  if (selection -> children && selection -> childCount > 0) //checks to see if the selection actually has a child menu to go to and has a childcount to ensure its indexed properly
  {
    currentMenu = selection->children; //array pointer to the current menu to navigate parent and sub menus
    selectedIndex = 0; //keeps track of the selected index to put the little indicator in the menu, 1 byte space saver. doubt we need more than 255 menu items
    topIndex = 0; //keeps track of the scrolling window index
    itemCount = selection->childCount; //total array size
  }
  else if (selection -> function) //if theres no child menu, check if theres a function
  {
    selection -> function(); //call the function
  } else
  {
    Serial.println("Placeholder so stuff doesnt break");
  }
}

void goBack() {
  if (currentMenu[0].parent) //if theres a parent, then continue
  {
    currentMenu = currentMenu[0].parent; // go back to parent menu array
    itemCount = sizeof(mainMenu)/sizeof(mainMenu[0]); //update the amount of items inside of the list
    selectedIndex = 0;//start at the top
    topIndex = 0;
  }
}


//=================================END OF MENU STUFF==============================================================




//=================================COMPASS UI STUFF==============================================================
struct handheld
{
  double my_lat; //this will pull the lattitude from the GPS module
  double my_long; //this will pull the longitude from the GPS module

  double my_angle; //stores the angle from north from the magnetometer
  double distance_to_node;//distance from the handheld to the node
  double angle_from_north; //gets the relative angle from north so the user can spin around and still get an accurate direction
  double angle_to_node;//angle from the handheld face to the node
};

struct node //node structure
{
  double node_lat;//pulls the lattidude from the gps
  double node_long;//same thing here
};

handheld my_handheld{0.6, 6.2, 0.0, 0.0, 0.0, 0.0};//hardcoded handheld data structure
node node_1{0.09, 0.0005}; //Hardcoded node instance structure to play around with



GFXcanvas16 canvas(240, 240);
int ui_state = 0; //0 for the compass, 1 for the menu
bool lastState = HIGH; //checks last state of the button

void getDistNAngle()
{
  double lat_diff;//difference in lattidude between node and handheld
  double long_diff;//difference in longitude
  double hypot;//hypoteneuse of the lattitude and longitude
  double hypot_m;//hypoteneuse in meters
  double angle_deg;
  lat_diff = node_1.node_lat - my_handheld.my_lat;//stationary target - moving object
  long_diff = node_1.node_long - my_handheld.my_long;

  long_diff = long_diff*cos(my_handheld.my_lat * 3.14159/180);//scale by the reference point because the lattidude line distances shrink the further away from the equator

  hypot = sqrt(sq(lat_diff)+sq(long_diff));//pythagorean theorem to find hypoteneuse

  angle_deg = atan2(long_diff, lat_diff)*180/3.14159;//arctan to find theta, then find the angle

  hypot_m = 111111*hypot;//conversion from coordinates to meters
  my_handheld.distance_to_node = hypot_m;//stores the hypoteneuse in meters into the handheld data structure
  my_handheld.angle_to_node = angle_deg;//stores the anglular direction
}

void drawDistance()
{
  tft.setCursor(75, 110);//sets the cursor to draw the text. x is from left to right byt a larger y is downward
  tft.setTextColor(GC9A01A_WHITE);
  tft.setTextSize(3);//scale the text to be bigger
  tft.print(my_handheld.distance_to_node, 2);//prints out the distance to 2 decimal places
}

void drawDirection()
{
  double bearing_to_node_deg;
  getBearingToNode(bearing_to_node_deg);
  double relative_ang = my_handheld.angle_from_north + bearing_to_node_deg;
  if (relative_ang < 0) relative_ang += 360;

  double x = 120 + 105*sin(relative_ang*3.1415/180);//120 is the origin, x uses sine to account for 90 degrees of rotation to get north. 
  double y = 120 - 105*cos(relative_ang*3.1415/180);//y coordinates face downward, so -cosine is used to account for downward face of the y axis and the 90 degree flip
  tft.fillCircle(x, y, 5, GC9A01A_WHITE);//creates a circle on the edge of the screen to show off the angle
}

void drawMagneticNorth()
{

  double x = 120 + 90*sin(my_handheld.my_angle*3.1415/180);//120 is the origin, x uses sine to account for 90 degrees of rotation to get north. 
  double y = 120 - 90*cos(my_handheld.my_angle*3.1415/180);//y coordinates face downward, so -cosine is used to account for downward face of the y axis and the 90 degree flip
  tft.fillCircle(x, y, 5, GC9A01A_WHITE);//creates a circle on the edge of the screen to show off the angle
}


void drawNotch()
{
  tft.fillCircle(120, 120, 115, OUTLINE_BLU);
  tft.fillCircle(120, 120, 109, GC9A01A_BLACK);
  tft.fillCircle(120, 120, 110, 0x1188);
  tft.fillCircle(120, 120, 100, OUTLINE_BLU);
  tft.fillCircle(120, 120, 95, GC9A01A_BLACK);
}
//=================================END OFCOMPASS UI STUFF==============================================================




//================== INTERRUPT===============================================================

// DEBOUNCER TEST

void gpio_handler(){
    if (gpio_get_irq_event_mask(BUTTON2) & GPIO_IRQ_EDGE_FALL){  // DEBOUNCE BUTTON TEST IRQ
        gpio_set_irq_enabled(BUTTON2, GPIO_IRQ_EDGE_FALL, false);
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        // Notify the task that the button was pressed
        vTaskNotifyGiveFromISR(taskDisplayNav, NULL);        // Yield to the task if it has higher priority
        vTaskNotifyGiveFromISR(taskDisplayMenu, NULL);        // Yield to the task if it has higher priority
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    if (gpio_get_irq_event_mask(BUTTON3) & GPIO_IRQ_EDGE_FALL) {
        gpio_set_irq_enabled(BUTTON3, GPIO_IRQ_EDGE_FALL, false);
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        // Notify the display control task
        vTaskNotifyGiveFromISR(taskDisplayMenu, NULL);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    if (gpio_get_irq_event_mask(BUTTON4) & GPIO_IRQ_EDGE_FALL) {
        gpio_set_irq_enabled(BUTTON4, GPIO_IRQ_EDGE_FALL, false);
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        // Notify the display control task
        vTaskNotifyGiveFromISR(taskDisplayMenu, NULL);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    if (gpio_get_irq_event_mask(BUTTON5) & GPIO_IRQ_EDGE_FALL) {
        gpio_set_irq_enabled(BUTTON5, GPIO_IRQ_EDGE_FALL, false);
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        // Notify the display control task
        vTaskNotifyGiveFromISR(taskDisplayMenu, NULL);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    if (gpio_get_irq_event_mask(BUTTON1) & GPIO_IRQ_EDGE_FALL) {
        gpio_set_irq_enabled(BUTTON1, GPIO_IRQ_EDGE_FALL, false);
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        // Notify the display control task
        vTaskNotifyGiveFromISR(taskDisplayMenu, NULL);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
//================== END OF INTERRUPT =============================



////=================================UI SWAPPING STUFF==============================================================

void menuControl()
{
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  //UP PRESSED
  if(gpio_get(BUTTON5) == 0){
      moveUp();
      drawMenu();
      while(gpio_get(BUTTON5) == 0) vTaskDelay(pdMS_TO_TICKS(5));
  }
  // DOWN PRESSED
  if(gpio_get(BUTTON4) == 0){
      moveDown();
      drawMenu();
      while(gpio_get(BUTTON4) == 0) vTaskDelay(pdMS_TO_TICKS(5));
  }

  if(gpio_get(BUTTON1) == 0){
      vTaskResume(taskDisplayCalibration);
      while(gpio_get(BUTTON1) == 0) vTaskDelay(pdMS_TO_TICKS(5));
  }

  vTaskDelay(pdMS_TO_TICKS(20));

  gpio_set_irq_enabled(BUTTON5, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(BUTTON4, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(BUTTON1, GPIO_IRQ_EDGE_FALL, true);
}

void controlNav(){
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  //UP PRESSED
  if (gpio_get(BUTTON2) == 0){
      vTaskResume(taskDisplayMenu);
      vTaskSuspend(taskDisplayNav);
      while(gpio_get(BUTTON2) == 0) vTaskDelay(pdMS_TO_TICKS(5));
  }
  gpio_set_irq_enabled(BUTTON2, GPIO_IRQ_EDGE_FALL, true);
}

void displayNav()
{
  tft.fillScreen(GC9A01A_BLACK);

  getDistNAngle();

  drawDistance();
  drawDirection();
  drawMagneticNorth();
  delay(1000);
}
//=================================END OF UI SWAPPING STUFF==============================================================


void vDisplayNav(void* pvParameters){
  vTaskSuspend(NULL);
  (void)pvParameters;
    tft.print("Nav Flag");
    vTaskDelay(pdMS_TO_TICKS(50));
    displayNav();
    while(1){
      controlNav();
      vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void vDisplayMenu(void* pvParameters){
  vTaskSuspend(NULL);
  (void)pvParameters;
  tft.print("MENU FLAG");
  vTaskDelay(pdMS_TO_TICKS(50));
  drawMenu();
  while(1){
      menuControl();
      vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void vDisplayControl(void* pvParameters){
  gpio_init(BUTTON1);
  gpio_init(BUTTON2);
  gpio_init(BUTTON3);
  gpio_init(BUTTON4);
  gpio_init(BUTTON5);

  gpio_set_dir(BUTTON1, GPIO_IN);
  gpio_set_dir(BUTTON2, GPIO_IN);
  gpio_set_dir(BUTTON3, GPIO_IN);
  gpio_set_dir(BUTTON4, GPIO_IN);
  gpio_set_dir(BUTTON5, GPIO_IN);

  gpio_pull_up(BUTTON1);
  gpio_pull_up(BUTTON2);
  gpio_pull_up(BUTTON3);
  gpio_pull_up(BUTTON4);
  gpio_pull_up(BUTTON5);

  gpio_set_irq_enabled(BUTTON1, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(BUTTON2, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(BUTTON3, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(BUTTON4, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(BUTTON5, GPIO_IRQ_EDGE_FALL, true);


  irq_add_shared_handler(IO_IRQ_BANK0, gpio_handler, PICO_SHARED_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY);
  irq_set_enabled(IO_IRQ_BANK0, true);
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(GC9A01A_BLACK);
  tft.setCursor(120, 120);


  for (uint8_t i = 0; i < sizeof(calibrateMenu) / sizeof(calibrateMenu[0]); i++)
  {
    calibrateMenu[i].parent = mainMenu;  // set parent pointer to the main menu. this will be used to go back
  }
  vTaskResume(taskDisplayNav);
  while(1){
    }
}

void vDisplayCalibration(void* pvParameters){
  vTaskSuspend(NULL);
  (void)pvParameters;
  tft.fillScreen(GC9A01A_BLACK);
  tft.setCursor(X_MENU_OFFSET, Y_MENU_OFFSET);
  tft.setTextSize(1.5);
  tft.print("Gyro Calibration");
  EventBits_t bits;
  tft.setCursor(X_OFFSET-50, Y_OFFSET);
  vTaskDelay(pdMS_TO_TICKS(50));

  
  xEventGroupSetBits(gyroEventGroup, GYRO_EVT_CALIBRATE_REQUEST); //This is breaking for some reason


  while(1)
  {
    tft.print("IN while loop");
    bits = xEventGroupGetBits(gyroEventGroup);
    std::string calstep = "";
    tft.setCursor(X_OFFSET, Y_MENU_OFFSET+30);
    tft.print("Entering if block");
    if (bits & GYRO_EVT_CALIBRATE_COMPLETE)
    {
      tft.print("Breaking");
      break;
    }
    else
    {
      tft.setCursor(X_OFFSET, Y_OFFSET);
      tft.print("Going into calibration");
      if (xQueueReceive(calQueue, &calstep, 0) == pdPASS)
      {
        printf("%s", calstep.c_str());
        tft.print(calstep.c_str());
      }
    }
  }

}