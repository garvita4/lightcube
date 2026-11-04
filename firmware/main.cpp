
#include <SPI.h>
#include <driver/i2s.h>

// ===================== PIN CONFIG =====================
#define LATCH_PIN   5     // RCLK of all 74HC595
#define OE_PIN      4     // Output Enable (active LOW) - optional
#define SPI_FREQ    8000000

// I2S Microphone pins (INMP441 style)
#define I2S_WS      15
#define I2S_SCK     14
#define I2S_SD      32

// ===================== CUBE DATA =====================
uint8_t cube[8][8];   // cube[layer][row]  → 8 bits for columns

// ===================== SETUP =====================
void setup() {
  Serial.begin(115200);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(OE_PIN, OUTPUT);
  digitalWrite(OE_PIN, LOW);   // Enable outputs

  SPI.begin();
  SPI.beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

  clearCube();
  Serial.println("Chronos LightCube Ready");
}

// ===================== MAIN LOOP =====================
void loop() {
  // Choose one effect:
  // rain();
  // wave();
  // planeBounce();
  // randomFill();
  // sparkle();
  // audioReactive();   // needs I2S mic

  rain();               // default demo
}

// ===================== CORE FUNCTIONS =====================

void clearCube() {
  memset(cube, 0, sizeof(cube));
}

void setVoxel(uint8_t x, uint8_t y, uint8_t z, bool state) {
  if (x > 7 || y > 7 || z > 7) return;
  if (state)
    cube[z][y] |= (1 << x);
  else
    cube[z][y] &= ~(1 << x);
}

void drawCube() {
  for (uint8_t layer = 0; layer < 8; layer++) {
    // Send 8 column bytes (one for each row)
    for (int8_t row = 7; row >= 0; row--) {
      SPI.transfer(cube[layer][row]);
    }

    // Send layer select byte (only one bit high)
    // TIP127 is PNP → we need to drive the selected layer LOW
    // So we send inverted layer select
    uint8_t layerByte = ~(1 << layer);
    SPI.transfer(layerByte);

    // Latch
    digitalWrite(LATCH_PIN, HIGH);
    digitalWrite(LATCH_PIN, LOW);

    delayMicroseconds(800);   // persistence time (tune this)
  }
}

// ===================== EFFECTS =====================

void rain() {
  static uint8_t drops[8][8];
  static unsigned long last = 0;

  if (millis() - last > 80) {
    last = millis();

    // Move drops down
    for (int z = 7; z > 0; z--) {
      for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
          if (bitRead(drops[z-1][y], x)) {
            bitSet(drops[z][y], x);
            bitClear(drops[z-1][y], x);
          }
        }
      }
    }

    // New drops at top
    for (int i = 0; i < 3; i++) {
      int x = random(8);
      int y = random(8);
      bitSet(drops[0][y], x);
    }

    // Copy to cube
    memcpy(cube, drops, sizeof(cube));
  }

  drawCube();
}

void wave() {
  static float phase = 0;
  phase += 0.15;

  clearCube();
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      float d = sqrt(pow(x-3.5,2) + pow(y-3.5,2));
      int z = 3.5 + 3.0 * sin(d * 0.8 - phase);
      if (z >= 0 && z < 8) setVoxel(x, y, z, true);
    }
  }
  drawCube();
}

void planeBounce() {
  static int z = 0;
  static int dir = 1;
  static unsigned long last = 0;

  if (millis() - last > 60) {
    last = millis();
    z += dir;
    if (z >= 7 || z <= 0) dir = -dir;
  }

  clearCube();
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++)
      setVoxel(x, y, z, true);

  drawCube();
}

void sparkle() {
  static unsigned long last = 0;
  if (millis() - last > 40) {
    last = millis();
    clearCube();
    for (int i = 0; i < 25; i++) {
      setVoxel(random(8), random(8), random(8), true);
    }
  }
  drawCube();
}

void randomFill() {
  static unsigned long last = 0;
  if (millis() - last > 30) {
    last = millis();
    setVoxel(random(8), random(8), random(8), random(2));
  }
  drawCube();
}

// ===================== I2S AUDIO (basic) =====================
void initI2S() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 256,
    .use_apll = false
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
}

void audioReactive() {
  // Simple volume → height effect
  static bool inited = false;
  if (!inited) {
    initI2S();
    inited = true;
  }

  int32_t samples[256];
  size_t bytes_read;
  i2s_read(I2S_NUM_0, samples, sizeof(samples), &bytes_read, portMAX_DELAY);

  long sum = 0;
  for (int i = 0; i < 128; i++) {
    sum += abs(samples[i] >> 14);
  }
  int level = constrain(sum / 800, 0, 7);

  clearCube();
  for (int z = 0; z <= level; z++) {
    for (int x = 0; x < 8; x++)
      for (int y = 0; y < 8; y++)
        setVoxel(x, y, z, true);
  }
  drawCube();
}