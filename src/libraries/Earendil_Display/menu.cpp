#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>
#include <SPI.h>

#define TFT_CS    25
#define TFT_DC     24
#define TFT_RST    12
#define TFT_MOSI 15
#define TFT_MISO 8
# define TFT_SCK 14

#define X_MENU_OFFSET 60
#define Y_MENU_OFFSET 60
//Global Variables

//Menu structure
struct MenuItem
{
  const char* name; //name of the menu item
  MenuItem* parent; //pointer to the parent item
  MenuItem* children;//points to the child submenu array whenever i get to doing that
  uint8_t childCount;//this is used for the loop to keep track of array size that way when going into the array it knows how far to scroll
  void (*function)();  // pointer to a function that runs when selected
};

//Placeholder reset function
void resetSystem()
{
  Serial.println("System resetting...");
}

//placeholder manual ping function
void manualPing()
{
  Serial.println("Manual ping sent");
}

void calMagMeter()
{
  Serial.println("Magnetometer Calibrated");
}

void calAltMeter()
{
  Serial.println("Altimeter Calibrated");
}

void calGyro()
{
  Serial.println("Altimeter Calibrated");
}

void calTemp()
{
  Serial.println("Temp Calibrated");
}


MenuItem calibrateMenu[] =
{
  {"Magnetometer", NULL, NULL, 0, calMagMeter}, {"Temperature", NULL, NULL, 0, calTemp}, {"Altimeter", NULL, NULL, 0, calAltMeter}, {"Gyroscope", NULL, NULL, 0, calGyro}
};
//Array of menu items
MenuItem mainMenu[] =
{
  {"Calibration", NULL, calibrateMenu, 4, NULL}, {"Info", NULL, NULL, 0, NULL}, {"Reset", NULL, NULL, 0, resetSystem}, {"Manual Ping", NULL, NULL, 0, manualPing}, {"Off", NULL, NULL, 0, NULL}
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
  tft.setTextSize(3);//scale the text to be bigger
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

void setup()
{
  Serial.begin(9600);
  for (uint8_t i = 0; i < sizeof(calibrateMenu) / sizeof(calibrateMenu[0]); i++)
  {
    calibrateMenu[i].parent = mainMenu;  // set parent pointer to the main menu. this will be used to go back
  }
  drawMenu();
}

void loop()
{
  if (Serial.available() > 0)//checks to make sure theres a character available to read inside of the buffer
  {
    char c = Serial.read(); //reads the keyboard input character from the serial monitor input
    switch(c) 
    {
      case 'w': moveUp(); drawMenu(); break;
      case 's': moveDown(); drawMenu(); break;
      case 'd': selectItem(); drawMenu(); break;
      case 'a': goBack(); drawMenu(); break;
    }
  }
}