#define ITERATIONS 20000

volatile uint8_t *sram_array = (uint8_t *)0x500;
const uint8_t flash_array[] PROGMEM = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};

unsigned long baseline_time = 0L;
unsigned long register_access_time = 0L;
unsigned long sram_access_time = 0L;
unsigned long flash_access_time = 0L;

void setup() {
  Serial.begin(9600);
  get_baseline_time();
  Serial.print("Baseline time: ");
  Serial.println(baseline_time);
  time_register_access(0, 1, 2, 3, 4, 5, 6, 7);
  Serial.print("Register access time: ");
  Serial.print(register_access_time);
  Serial.print("  ");
  Serial.print(((float)((long)(register_access_time - baseline_time)))/(32L*ITERATIONS), 5);
  Serial.println("us per read");
  time_sram_access();
  Serial.print("SRAM access time: ");
  Serial.print(sram_access_time);
  Serial.print("  ");
  Serial.print(((float)((long)(sram_access_time - baseline_time)))/(32L*ITERATIONS), 5);
  Serial.println("us per read");
  time_flash_access();
  Serial.print("Flash access time: ");
  Serial.print(flash_access_time);
  Serial.print("  ");
  Serial.print(((float)((long)(flash_access_time - baseline_time)))/(32L*ITERATIONS), 5);
  Serial.println("us per read");
}

void loop() {
  ;
}

uint8_t get_baseline_time() {
  Serial.print("Obtaining baseline time ");
  unsigned long start, stop;
  register uint8_t sum = 0;
  for (int i = 0; i < ITERATIONS; i++) {
    start = micros();
    for (int j = 0; j < 32; j++) {
      sum += j;
    }
    stop = micros();
    baseline_time += (stop - start);
    if (!(i % 1000)) {
      Serial.print(". ");
    }
  }
  Serial.println();
  return sum;
}

uint8_t time_register_access(
  uint8_t a0, uint8_t a1, uint8_t a2, uint8_t a3,
  uint8_t a4, uint8_t a5, uint8_t a6, uint8_t a7
) {
  Serial.print("Measuring register access time ");
  unsigned long start, stop;
  register uint8_t sum = 0;
  for (int i = 0; i < ITERATIONS; i++) {
    start = micros();
    for (int j = 0; j < 4; j++) {
      sum += a0 + j;
      sum += a1 + j;
      sum += a2 + j;
      sum += a3 + j;
      sum += a4 + j;
      sum += a5 + j;
      sum += a6 + j;
      sum += a7 + j;
    }
    stop = micros();
    register_access_time += (stop - start);
    if (!(i % 1000)) {
      Serial.print(". ");
    }
  }
  Serial.println();
  return sum;
}

uint8_t time_sram_access() {
  Serial.print("Measuring SRAM access time ");
  unsigned long start, stop;
  register uint8_t sum = 0;
  for (int i = 0; i < ITERATIONS; i++) {
    start = micros();
    for (int j = 0; j < 4; j++) {
      sum += *(sram_array + j);
      sum += *(sram_array + j + 4);
      sum += *(sram_array + j + 8);
      sum += *(sram_array + j + 12);
      sum += *(sram_array + j + 16);
      sum += *(sram_array + j + 20);
      sum += *(sram_array + j + 24);
      sum += *(sram_array + j + 28);
    }
    stop = micros();
    sram_access_time += (stop - start);
    if (!(i % 1000)) {
      Serial.print(". ");
    }
  }
  Serial.println();
  return sum;
}

uint8_t time_flash_access() {
  Serial.print("Measuring flash access time ");
  unsigned long start, stop;
  register uint8_t sum = 0;
  for (int i = 0; i < ITERATIONS; i++) {
    start = micros();
    for (int j = 0; j < 4; j++) {
      sum += (uint8_t)pgm_read_byte(flash_array + j);
      sum += (uint8_t)pgm_read_byte(flash_array + j + 4);
      sum += (uint8_t)pgm_read_byte(flash_array + j + 8);
      sum += (uint8_t)pgm_read_byte(flash_array + j + 12);
      sum += (uint8_t)pgm_read_byte(flash_array + j + 16);
      sum += (uint8_t)pgm_read_byte(flash_array + j + 20);
      sum += (uint8_t)pgm_read_byte(flash_array + j + 24);
      sum += (uint8_t)pgm_read_byte(flash_array + j + 28);
    }
    stop = micros();
    flash_access_time += (stop - start);
    if (!(i % 1000)) {
      Serial.print(". ");
    }
  }
  Serial.println();
  return sum;
}
