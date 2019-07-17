/* Simple DataFlash - see http://www.technoblogy.com/show?2JMU

   David Johnson-Davies - www.technoblogy.com - 17th July 2019
   
   CC BY 4.0
   Licensed under a Creative Commons Attribution 4.0 International license: 
   http://creativecommons.org/licenses/by/4.0/
*/

// Winbond DataFlash commands
#define PAGEPROG      0x02
#define READSTATUS    0x05
#define READDATA      0x03
#define WRITEENABLE   0x06
#define CHIPERASE     0x60
#define READID        0x90

// Arduino Uno pins used for dataflash
const int sck = 13, miso = 12, mosi = 11, cs = 10, power = 9;

class DF {
  public:
    boolean Setup();
    void BeginRead(unsigned long addr);
    void BeginWrite(void);
    uint8_t ReadByte(void);
    void WriteByte(uint8_t data);
    void EndRead(void);
    void EndWrite(void);
  private:
    unsigned long addr;
    uint8_t Read(void);
    void Write(uint8_t);
    void Busy(void);
    void WriteEnable(void);
};

boolean DF::Setup () {
  uint8_t manID, devID;
  pinMode(power, OUTPUT); digitalWrite(power, HIGH);
  digitalWrite(cs, HIGH); pinMode(cs, OUTPUT); 
  pinMode(sck, OUTPUT);
  pinMode(mosi, OUTPUT);
  pinMode(miso, INPUT);
  digitalWrite(sck, LOW); digitalWrite(mosi, HIGH);
  delay(1);
  digitalWrite(cs, LOW);
  delay(100);
  Write(READID); Write(0);Write(0);Write(0);
  manID = Read();
  devID = Read();
  digitalWrite(cs, HIGH);
  return (devID == 0x15); // Found correct device
}

void DF::Write (uint8_t data) {
  shiftOut(mosi, sck, MSBFIRST, data);
}

void DF::Busy () {
  digitalWrite(cs, 0);
  Write(READSTATUS);
  while (Read() & 1 != 0);
  digitalWrite(cs, 1);
}

void DF::WriteEnable () {
  digitalWrite(cs, 0);
  Write(WRITEENABLE);
  digitalWrite(cs, 1);
}

void DF::BeginRead (unsigned long start) {
  addr = start;
  Busy();
  digitalWrite(cs, 0);
  Write(READDATA);
  Write(addr>>16);
  Write(addr>>8);
  Write(addr);
}

uint8_t DF::Read () {
  return shiftIn(miso, sck, MSBFIRST);
}

void DF::EndRead(void) {
  digitalWrite(cs, 1);
}

void DF::BeginWrite () {
  addr = 0;
  Busy();
  // Erase DataFlash
  WriteEnable();
  digitalWrite(cs, 0);
  Write(CHIPERASE);
  digitalWrite(cs, 1);
  Busy();
}

uint8_t DF::ReadByte () {
  return Read();
}

void DF::WriteByte (uint8_t data) {
  // New page
  if ((addr & 0xFF) == 0) {
    digitalWrite(cs, 1);
    Busy();
    WriteEnable();
    digitalWrite(cs, 0);
    Write(PAGEPROG);
    Write(addr>>16);
    Write(addr>>8);
    Write(0);
  }
  Write(data);
  addr++;
}

void DF::EndWrite (void) {
  digitalWrite(cs, 1);
}

DF DataFlash;

// Test program
void setup() {
  Serial.begin(9600);
  while (!Serial);  
  if (!DataFlash.Setup()) {
    Serial.println("DataFlash not found");
    for(;;);
  }
}


void loop() {
  unsigned long Test = 400000;
  Serial.println("Erase");
  DataFlash.BeginWrite();
  randomSeed(12345);
  Serial.println("Write");
  for (unsigned long i=0; i<Test; i++) {
    DataFlash.WriteByte(random(256));
  }
  DataFlash.EndWrite();

  Serial.println("Read");
  DataFlash.BeginRead(0);
  randomSeed(12345);
  int errors = 0;
  for (unsigned long i=0; i<Test; i++) {
    if (DataFlash.ReadByte() != random(256)) errors++;
  }
  DataFlash.EndRead();
  Serial.println("End");
  Serial.print(errors); Serial.println(" errors");
  for(;;);
}
