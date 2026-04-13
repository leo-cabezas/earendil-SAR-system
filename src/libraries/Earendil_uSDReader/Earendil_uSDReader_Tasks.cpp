#include <Earendil_uSDReader.h> // ATTENTION: Add all library dependencies to this header.

SdExFat sd;
ExFile file;   // or file_t depending on your setup

void vMicroSD(void* pvParameters) {
  (void) pvParameters;

  if (!sd.begin(USD_CS)) {
    Serial.println("SD init failed!");
    while(1);
  }

  // Open file
  if (!file.open("test.txt", O_WRONLY | O_CREAT | O_APPEND)) {
    Serial.println("Open failed");
    while(1);
  }

  // Write data
  file.println("Hello from File_t!");
  file.print("Number: ");
  file.println(88);

  file.close();

  while(1);
}
