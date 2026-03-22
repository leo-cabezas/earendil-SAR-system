#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>
#include <SPI.h>

#define TFT_CS    25
#define TFT_DC     24
#define TFT_RST    12
#define TFT_MOSI 15
#define TFT_MISO 8
# define TFT_SCK 14

#define X_OFFSET 120
#define Y_OFFSET 120
//Global Variables
#define OUTLINE_BLU 0x2CB6
#define OUTLINE_DARKBLU 0x1188

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

handheld my_handheld{0.0, 0.0, 0.0, 0.0};//hardcoded handheld data structure
node node_1{0.0005, 0.0005}; //Hardcoded node instance structure to play around with


Adafruit_GC9A01A tft = Adafruit_GC9A01A(TFT_CS, TFT_DC, TFT_MOSI,TFT_SCK, TFT_RST, TFT_MISO);//initialization for the adafruit screen structure
GFXcanvas16 canvas(240, 240);

void setup() {
  tft.begin();
  tft.setRotation(0);
}

void get_dist_n_angle()
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

void draw_distance()
{
  tft.setCursor(75, 110);//sets the cursor to draw the text. x is from left to right byt a larger y is downward
  tft.setTextColor(GC9A01A_WHITE);
  tft.setTextSize(3);//scale the text to be bigger
  tft.print(my_handheld.distance_to_node, 2);//prints out the distance to 2 decimal places
}

void draw_direction()
{
  double x = 120 + 105*sin(my_handheld.angle_to_node*3.1415/180);//120 is the origin, x uses sine to account for 90 degrees of rotation to get north. 
  double y = 120 - 105*cos(my_handheld.angle_to_node*3.1415/180);//y coordinates face downward, so -cosine is used to account for downward face of the y axis and the 90 degree flip
  tft.fillCircle(x, y, 5, GC9A01A_WHITE);//creates a circle on the edge of the screen to show off the angle
}



void draw_notch()
{
  tft.fillCircle(120, 120, 115, OUTLINE_BLU);
  tft.fillCircle(120, 120, 109, GC9A01A_BLACK);
  tft.fillCircle(120, 120, 110, 0x1188);
  tft.fillCircle(120, 120, 100, OUTLINE_BLU);
  tft.fillCircle(120, 120, 95, GC9A01A_BLACK);
}


void loop()
{

  tft.fillScreen(GC9A01A_BLACK);

  get_dist_n_angle();


  draw_distance();
  draw_direction();
  delay(1000);

}
