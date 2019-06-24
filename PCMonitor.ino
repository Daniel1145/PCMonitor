#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>   // Hardware-specific library
MCUFRIEND_kbv tft;

#include <Fonts/FreeSans12pt7b.h>

#define BLACK   0x0000
#define WHITE   0xFFFF

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];

char cpuUsage[numChars];
char ramUsed[numChars];
char ramTotal[numChars];
char gpuUsage[numChars];
char gpuTemp[numChars];
char* ramUsage;
char* gpuTotal;

boolean newData = false;

void setup() {
  Serial.begin(9600);
  int16_t ID = tft.readID();
  if (ID == 0xD3) ID = 0x9481;
  tft.begin(ID);
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  tft.setFont(&FreeSans12pt7b);
  tft.setTextColor(WHITE);
  tft.setTextSize(1.3);
    
  tft.setCursor(10,30);
  tft.print("CPU");
  tft.setCursor(63,28);
  tft.print(":");

  tft.setCursor(10,60);
  tft.print("RAM");
  tft.setCursor(63,58);
  tft.print(":");

  tft.setCursor(10,90);
  tft.print("GPU");
  tft.setCursor(63,88);
  tft.print(":");
}

void loop() {
  receiveData();
  if (newData) {
    strcpy(tempChars, receivedChars);
    parseData();
    displayData();
    newData = false;
    free(ramUsage);
  }
  delay(100);
}

void receiveData() {
  static boolean inProgress = false;
  static byte index = 0;
  char startMarker = '*';
  char endMarker = '^';
  char curr;

  while (Serial.available() > 0 && newData == false) {
    curr = Serial.read();

    if (inProgress) {
      if (curr != endMarker) {
        receivedChars[index] = curr;
        index++;
        
      }
      else {
        receivedChars[index] = '\0';
        inProgress = false;
        index = 0;
        newData = true;
      }
    } 
    else if (curr == startMarker) inProgress = true;
  }
}

void parseData() {
  char* index;

  index = strtok(tempChars, ",");
  strcpy(cpuUsage, index);
  strcat(cpuUsage, "%");

  index = strtok(NULL, ",");
  strcpy(ramUsed, index);
  strcat(ramUsed, "/");

  index = strtok(NULL, ",");
  strcpy(ramTotal, index);
  strcat(ramTotal, " GB");

  index = strtok(NULL, ",");
  strcpy(gpuTemp, index);
  strcat(gpuTemp, "C");

  index = strtok(NULL, ",");
  strcpy(gpuUsage, index);
  strcat(gpuUsage, "%, ");

  ramUsage = malloc(strlen(ramUsed) + strlen(ramTotal) + 1);
  memcpy(ramUsage, ramUsed, strlen(ramUsed));
  memcpy(ramUsage+strlen(ramUsed), ramTotal, strlen(ramTotal)+1);

  gpuTotal = malloc(strlen(gpuUsage) + strlen(gpuTemp) + 1);
  memcpy(gpuTotal, gpuUsage, strlen(gpuUsage));
  memcpy(gpuTotal+strlen(gpuUsage), gpuTemp, strlen(gpuTemp) + 1); 
}

void displayData() {
  static int16_t CPUx = 70;
  static int16_t CPUy = 30;
  static uint16_t CPUw, CPUh;
  
  static int16_t ramUsagex = 70;
  static int16_t ramUsagey = 60;
  static uint16_t ramUsagew, ramUsageh;

  static int16_t GPUx = 70;
  static int16_t GPUy = 90;
  static int16_t GPUw, GPUh;

  tft.fillRect(CPUx, CPUy, CPUw, CPUh, BLACK);
  tft.fillRect(ramUsagex, ramUsagey, ramUsagew, ramUsageh, BLACK);
  tft.fillRect(GPUx, GPUy, GPUw, GPUh, BLACK);

  tft.setCursor(70,30);
  tft.print(cpuUsage);

  tft.setCursor(70,60);
  tft.print(ramUsage);

  tft.setCursor(70,90);
  tft.print(gpuTotal);
  
  tft.getTextBounds(cpuUsage, 70, 30, &CPUx, &CPUy, &CPUw, &CPUh);
  tft.getTextBounds(ramUsage, 70, 60, &ramUsagex, &ramUsagey, &ramUsagew, &ramUsageh);
  tft.getTextBounds(gpuTotal, 70, 90, &GPUx, &GPUy, &GPUw, &GPUh);
}
