
//---------------------------------------------------------------//
//
//          @@@@@@@                          @@@              ,@@@
//       @@@@*   @@@@*                       @@@              ,@@@
//       @@@       @@@    @@@@@@@    @@@@@@@ @@@   @@@@@@@@   ,@@@
//        @@@@@@@@@*    @@@     @@@  @@@@    @@@ /@@@    @@@  ,@@@
//               @@@@@ &@@@@@@@@@@@# @@@     @@@   @@@@@@@@@  ,@@@
//      ,@@@       @@@  @@@     %%%  @@@     @@@ @@@     @@@  ,@@@
//        @@@@@@@@@@@    @@@@@@@@@@  @@@     @@@ #@@@@@@@@@@  ,@@@
//
//
//  @@@@@@@@@
//@@@@     @@@@
//@@@@            ,@@@@@@@@   @@@@@@@@@@   @@@@@@@@@   %@@@@@@@@   @@@@@@@
//  @@@@@@@@@    @@@     @@@  @@@     @@@ @@@    @@@. @@@     @@@  @@@
//         (@@@  @@@@@@@@@@@@ @@@     @@@  %@@@@@@@@  @@@     *@@@ @@@
//@@@       @@@* @@@     @@@/ @@@     @@@ @@@     @@@ @@@     @@@  @@@
// @@@@@@@@@@@    @@@@@@@@@.  @@@     @@@  @@@@@@@@@   @@@@@@@@@   @@@
//
//                Example code for reading currently all
//                          available sensors
//--------------------------------------------------------------//

//--------------------------------------------------------------//
// Sensor ID's from the Sensor's details view within the App
//--------------------------------------------------------------//
const byte ACCELEROMETER        = 1;
const byte MAGNETIC_FIELD       = 2;
const byte GYROSCOPE            = 4;
const byte LIGHT                = 5;
const byte PRESSURE             = 6;
const byte PROXIMITY            = 8;
const byte GRAVITY              = 9;
const byte RELATIVE_HUMIDITY    = 12;
const byte TEMPERATURE          = 13;
const byte HEART_BEAT           = 31;
const byte ORIENTATION          = 99;
const byte LINE_SENSOR          = 100;
const byte POSITION             = 101;
const byte PARAMETER            = 103;
const byte SPEED                = 105;

//--------------------------------------------------------------//
//Other constants & definitions
//--------------------------------------------------------------//
const byte POSITION_DATA_LENGTH     = 4;
const byte POSITION_BYTES_PER_FLOAT = 3;
const byte REGULAR_DATA_LENGTH      = 3;
const byte REGULAR_BYTES_PER_FLOAT  = 4;
const byte FRAME_LENGTH             = 13;
//union for conversion of 4 bytes to a float value
union uByteFloat {
  byte b[4];
  float fval;
} uBF;

//--------------------------------------------------------------//
//Global variables
//--------------------------------------------------------------//
float           g_receivedPositionData[POSITION_DATA_LENGTH] = {0.F};
float           g_receivedRegularData[REGULAR_DATA_LENGTH] = {0.F};
byte            g_receivedParameterData[FRAME_LENGTH] = {0};
byte            g_receivedRawData[FRAME_LENGTH] = {0};

void setup() {
  Serial.begin(250000);
  while (!Serial);
}

// loop method does the following:
// 1) wait for a complete frame to arrive
// 2) check first byte (= sensor id) and interprete the payload of 12bytes
//    as described within the sensor details view within the app (see output frame layout section)
void loop() {

  //Wait for new frame to be fully received
  if (Serial.available() >= FRAME_LENGTH) {
    for (int i = 0; i < FRAME_LENGTH; i++) {
      g_receivedRawData[i] = Serial.read();
    }

    //first received byte holds the sensor id
    const byte sensorId = g_receivedRawData[0];

    switch (sensorId) {
      case ACCELEROMETER:
      case MAGNETIC_FIELD:
      case GYROSCOPE:
      case LIGHT:
      case PRESSURE:
      case PROXIMITY:
      case GRAVITY:
      case RELATIVE_HUMIDITY:
      case TEMPERATURE:
      case HEART_BEAT:
      case LINE_SENSOR:
      case ORIENTATION:
      case SPEED:
        {
          //all these sensors do send up to 3 float values. So every 4 bytes are interpreted as a float
          for (int i = 0; i < REGULAR_DATA_LENGTH; i++) {
            g_receivedRegularData[i] = getFloatFrom4Bytes(g_receivedRawData, i);
          }
          break;
        }
      case POSITION:
        {
          //Position sensor sends 4 fixed point values (Position x,y and the velocity as well as the yaw angle)
          //each of these values uses 3 bytes, these 3 bytes are transformed into 3 floats here
          for (int i = 0; i < POSITION_DATA_LENGTH; i++) {
            g_receivedPositionData[i] = getFloatFrom3Bytes(g_receivedRawData, i);
          }
          break;
        }
      case PARAMETER:
        {
          //parameter sensor sends user defined values, how this is interpreted is up to you
          //So just copy everything
          for (int i = 0; i < FRAME_LENGTH; i++) {
            g_receivedParameterData[i] = g_receivedRawData[i];
          }
          break;
        }
      default:
        {
          //not supported sensor id received, do nothing
        }
    }

    //use the received data here!

  }
}
// getFloatFrom4Bytes method - gets a float value from 3byte fixed point vlaue
// Input:
// * receivedBytes - received byte array
// * dataIndex - current data index
// Returns: nothing
float getFloatFrom4Bytes(const byte receivedBytes[], const byte dataIndex) {
  uBF.b[0] = receivedBytes[1 + dataIndex * REGULAR_BYTES_PER_FLOAT];
  uBF.b[1] = receivedBytes[2 + dataIndex * REGULAR_BYTES_PER_FLOAT];
  uBF.b[2] = receivedBytes[3 + dataIndex * REGULAR_BYTES_PER_FLOAT];
  uBF.b[3] = receivedBytes[4 + dataIndex * REGULAR_BYTES_PER_FLOAT];
  return uBF.fval;
}


// getFloatFrom3Bytes method - gets a float value from 3byte fixed point vlaue
// Input:
// * receivedBytes - received byte array
// * dataIndex - current data index
// Returns: nothing
float getFloatFrom3Bytes(const byte receivedBytes[], const byte dataIndex) {
  long intRepresentation = (
                             ((0xFFL & receivedBytes[3 + dataIndex * POSITION_BYTES_PER_FLOAT]) << 16) |
                             ((0xFFL & receivedBytes[2 + dataIndex * POSITION_BYTES_PER_FLOAT]) << 8)  |
                             ((0xFFL & receivedBytes[1 + dataIndex * POSITION_BYTES_PER_FLOAT]))
                           );

  if ((intRepresentation & 0x00800000) > 0) {
    intRepresentation |= 0xFF000000;
  } else {
    intRepresentation &= 0x00FFFFFF;
  }
  //65536.F is the norming factor of the fixed point value
  return intRepresentation / 65536.F;
}
