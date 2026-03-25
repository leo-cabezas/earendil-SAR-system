#include <Earendil_uSDReader.h>

#define DISABLE_FS_H_WARNING
#include <SdFat.h>

SdExFat sd;
ExFile file;   // or file_t depending on your setup

void vMicroSD() {
  (void) pvParameters;

  if (!sd.begin(13)) {
    Serial.println("SD init failed!");
    return;
  }

  // Open file
  if (!file.open("test.txt", O_WRONLY | O_CREAT | O_APPEND)) {
    Serial.println("Open failed");
    return;
  }

  // Write data
  file.println("Hello from File_t!");
  file.print("Number: ");
  file.println(88);

  file.close();

  while(1);
}
