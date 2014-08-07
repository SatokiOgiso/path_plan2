// -*- C++ -*-
/*!
 * @file  path_plan2.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include "path_plan2.h"
#include "iostream"

//#define DEBUG_
#define DEBUG_MAP  // show map
#define DEBUG_CMAP // make cleaned map manually
//#define DEBUG_OMAP // make occupied map manually

using namespace std;

// Module specification
// <rtc-template block="module_spec">
static const char* path_plan2_spec[] =
  {
    "implementation_id", "path_plan2",
    "type_name",         "path_plan2",
    "description",       "ModuleDescription",
    "version",           "1.0.0",
    "vendor",            "VenderName",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.velocityCoeff", "1.0",
    "conf.default.angularVelocityCoeff", "1.0",
    // Widget
    "conf.__widget__.velocityCoeff", "text",
    "conf.__widget__.angularVelocityCoeff", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
path_plan2::path_plan2(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_robotPoseIn("robotPose", m_robotPose),
    m_obstacleDataIn("obstacleData", m_obstacleData),
    m_wayPointOut("wayPoint", m_wayPoint)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
path_plan2::~path_plan2()
{
}



RTC::ReturnCode_t path_plan2::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("robotPose", m_robotPoseIn);
  addInPort("obstacleData", m_obstacleDataIn);
  
  // Set OutPort buffer
  addOutPort("wayPoint", m_wayPointOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("velocityCoeff", m_velocityCoeff, "1.0");
  bindParameter("angularVelocityCoeff", m_angularVelocityCoeff, "1.0");

  // </rtc-template>

  cout << "initialized " << endl;
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t path_plan2::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t path_plan2::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t path_plan2::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t path_plan2::onActivated(RTC::UniqueId ec_id)
{
  for(int i=0; i<MAPGRIDNUMX ; i++){ //initialize cleaned map with 0
    for(int j=0; j<MAPGRIDNUMY ; j++){
      cleanedMap[i][j] = 0;
    }
  }
  robotLocation[0] = 0;
  robotLocation[1] = 0;
  robotLocationGrid[0] = 5;
  robotLocationGrid[1] = 5;
  robotHeading = 0;
  robotHeadingGrid = 1;
  nextWayPoint[0] = 0;
  nextWayPoint[1] = 0;
  nextWayPointGrid[0] = 5;
  nextWayPointGrid[1] = 5;
  
  cout << "activated " << endl;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t path_plan2::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


inline void path_plan2::makeStepsMap(int zeroLocation[2], int occupiedMap[MAPGRIDNUMX][MAPGRIDNUMY] ,int stepsMap[MAPGRIDNUMX][MAPGRIDNUMY])
{
  int ct = 0; // count for map update
  int pt0 = 0; // current steps from zero location ( = goal )

  // initialize steps map
  for(int i=0; i<MAPGRIDNUMX; i++){ // loop for x axis
    for(int j=0; j<MAPGRIDNUMY; j++){ // loop for y axis
      stepsMap[i][j] = -1;// initial value is non-positive number
    }
  }

  stepsMap[zeroLocation[0]][zeroLocation[1]] = 0; // set goal
  
  do{ // make steps map. loop while map is updated.
    ct = 0; //initialize updates counter
    for(int i=0; i<MAPGRIDNUMX; i++){ // loop for x axis
      for(int j=0; j<MAPGRIDNUMY; j++){ // loop for y axis
        if(stepsMap[i][j] == pt0){ // if current step number from zero location was detected
          if(i+1 < MAPGRIDNUMX){// if (x+1, y) is in the map
            if(occupiedMap[i+1][j] == 0){// if the location is not occupied
              if(stepsMap[i+1][j] == -1){ // if (x+1, y) has initial value
                stepsMap[i+1][j] = pt0+1; // set next step number to the neighbor
                ct ++;  // count update.
              }
            }
          }
          if(j+1 < MAPGRIDNUMY){// if (x, y+1) is in the map
            if(occupiedMap[i][j+1] == 0){// if the location is not occupied
              if(stepsMap[i][j+1] == -1){ // if (x, y+1) has initial value
                stepsMap[i][j+1] = pt0+1; // set next step number to the neighbor
                ct ++; // count update.
              }
            }
          }
          if(i-1 >= 0){// if (x-1, y) is in the map
            if(occupiedMap[i-1][j] == 0){// if the location is not occupied
              if(stepsMap[i-1][j] == -1){ // if (x-1, y) has initial value
                stepsMap[i-1][j] = pt0+1; // set next step number to the neighbor
                ct ++; // count update
              }
            }
          }
          if(j-1 >= 0){// if (x, y-1) is in the map
            if(occupiedMap[i][j-1] == 0){// if the location is not occupied
              if(stepsMap[i][j-1] == -1){ // if (x, y-1) has initial value
                stepsMap[i][j-1] = pt0+1; // set next step number to the neighbor
                ct ++; // count update
              }
            }
          }
        }
      }
    }
    pt0 ++;
  }while(ct != 0);
}

inline void path_plan2::displayStepsMap(int stepsMap[MAPGRIDNUMX][MAPGRIDNUMY])
{
  for(int i=0; i<MAPGRIDNUMX; i++){
    for(int j=0; j<MAPGRIDNUMY; j++){
      cout << stepsMap[i][j] << "\t ";
    }
    cout << endl;
  }
  cout << endl;
}

inline int path_plan2::determineNextGoal(int nextGoal[2], int stepsMap[MAPGRIDNUMX][MAPGRIDNUMY], int cleanedMap[MAPGRIDNUMX][MAPGRIDNUMY])
{
  int candidateSteps = 9999; //initialize next candidate steps
  int goalValid = 0; // flag to show validity of goal location

  for(int i=0; i<MAPGRIDNUMX; i++){
    for(int j=0; j<MAPGRIDNUMY; j++){
      if(cleanedMap[i][j] == 0){ // the location is not cleaned
        if(stepsMap[i][j] > 0 && /* the robot can go the grid */ \
           candidateSteps > stepsMap[i][j]){ // the grid is nearer than last candidate
          nextGoal[0] = i;
          nextGoal[1] = j;
          candidateSteps = stepsMap[i][j];
          goalValid = 1;
        }
      }
    }
  }

  return goalValid;
}

inline int path_plan2::determineNextWayPoint(int nextWayPoint[2], int stepsMap[MAPGRIDNUMX][MAPGRIDNUMY],int heading)
{
  int goalValid = 1;
  
  

  return goalValid;
}

inline int path_plan2::realToGrid(int locationGrid[2], double location[2])
{
  int gridValid = 1;
  int locationGridOld[2] = {locationGrid[0], locationGrid[1]};
    
  locationGrid[0] = (int)((location[0] - XORG)/MAPDIV); // calculate grid number for x
  locationGrid[1] = (int)((location[1] - YORG)/MAPDIV); // calculate grid number for y 

  if((MAPGRIDNUMX <= locationGrid[0]) || (MAPGRIDNUMY <= locationGrid[1]) || (0 > locationGrid[0]) || (0 > locationGrid[1])){
    gridValid = 0; // the calculated grid is out of map
    locationGrid[0] = locationGridOld[0];
    locationGrid[1] = locationGridOld[1];
  }

  return gridValid;
}

inline void path_plan2::gridToReal(double location[2], int locationGrid[2])
{
  location[0] = ((double)locationGrid[0]) * MAPDIV + XORG;
  location[1] = ((double)locationGrid[1]) * MAPDIV + YORG;
}

inline int headingToGrid(double heading)
{
  int headingGrid = 0;
  double  myPI4=0.785375; // pi/4

  if((heading >= 0 && heading <= myPI4) || (heading >= myPI4*7 && heading <= myPI4 * 8 )) headingGrid = 1;
  if(heading > myPI4 && heading <= myPI4*3) headingGrid = 2;
  if(heading > myPI4*3 && heading <= myPI4*5) headingGrid = 3;
  if(heading > myPI4*5 && heading <= myPI4*7) headingGrid = 4;
  
  return headingGrid;
}

RTC::ReturnCode_t path_plan2::onExecute(RTC::UniqueId ec_id)
{
  int robotGridValid = 0;// check whether obtained robot location is in the map. 0:not-in-map, 1: in-map(valid)
  int goalValid = 0; // check whether obtained goal is valid or not 0:not-valid, 1:valid
  int wayPointValid = 0; // check whether obtained way point is valid. 0:not-valid, :valid
  int nextGoal[2] = {0, 0}; // next goal on grid (x, y)
  int occupiedMap[MAPGRIDNUMX][MAPGRIDNUMY];// occupied map made from URG data. 0:not occupied, 1:occupied
  int stepsMapFromRobot[MAPGRIDNUMX][MAPGRIDNUMY];  //steps map from robot
                                                    //-1:initial value. no route to the grid.
                                                    // 0:goal
                                                    //>0:steps to reach the grid from goal.
  int stepsMapFromGoal[MAPGRIDNUMX][MAPGRIDNUMY];   //steps map from goal
                                                    //-1:initial value. no route to the grid.
                                                    // 0:goal
                                                    //>0:steps to reach the grid from goal.

  

  if(m_robotPoseIn.isNew()){ // if robot location data is available
    m_robotPoseIn.read();    // read the pose data
    robotLocation[0] = m_robotPose.data.position.x; // store current robot x location 
    robotLocation[1] = m_robotPose.data.position.y; // store current robot y location
    robotGridValid = realToGrid(robotLocationGrid, robotLocation); //check calidation of robot position
    robotHeading = m_robotPose.data.heading; // store current robot heading (rad)
    cleanedMap[robotLocationGrid[0]][robotLocationGrid[1]] = 1; // set cleaned on cleaned map
    cout << "grid:" << robotLocationGrid[0]<< ", " << robotLocationGrid[1] << endl; // display current grids
  }

#ifdef DEBUG_ 
    robotLocation[0] = 0; // store current robot x location 
    robotLocation[1] = 0; // store current robot y location
    robotGridValid = realToGrid(robotLocationGrid, robotLocation);
    cleanedMap[robotLocationGrid[0]][robotLocationGrid[1]] = 1; // set cleaned on cleaned map
    //cout << "grid:" << robotLocationGrid[0] << ", " << robotLocationGrid[1] << endl; // display current grids
#endif

  if(robotLocationGrid[0] == nextWayPointGrid[0] && robotLocationGrid[1] == nextWayPointGrid[1]){ // if robot entered the next way point grid
#ifdef DEBUG_OMAP
    //make occupied map by hand
    for(int i=0; i<MAPGRIDNUMX ; i++){ //initialize map with 0
      for(int j=0; j<MAPGRIDNUMY ; j++){
        occupiedMap[i][j] = 0;
      }
    }
    occupiedMap[3][3]=1;
    occupiedMap[3][4]=1;
    occupiedMap[5][3]=1;
    occupiedMap[4][3]=1;
    occupiedMap[4][4]=1;
    occupiedMap[4][5]=1;
    occupiedMap[4][6]=1;
#endif

    cout << "grid:" << robotLocationGrid[0]<< ", " << robotLocationGrid[1] << endl; 

    makeStepsMap(robotLocationGrid, occupiedMap, stepsMapFromRobot);// make steps map from robot

    // determine next goal from steps map from robot & cleaned map
    goalValid = determineNextGoal(nextGoal, stepsMapFromRobot, cleanedMap); // make next goal from robot

    cout << "next goal:" << nextGoal[0] << ", " << nextGoal[1] << endl;
    cout << "valid? :" << goalValid << endl; 

    // make steps map from goal
    makeStepsMap(nextGoal, occupiedMap, stepsMapFromGoal);

    robotHeadingGrid = headingToGrid(robotHeading);
    // determine next way point
    nextWayPointGrid[0] = nextGoal[0];
    nextWayPointGrid[1] = nextGoal[1];
    //wayPointValid = determineNextWayPoint(nextWayPointGrid, stepsMapFromGoal, robotHeadingGrid); 
    cout << "next way point:" << nextWayPointGrid[0] << ", " << nextWayPointGrid[1] << endl;
    cout << "valid? :" << wayPointValid << endl;

    gridToReal(nextWayPoint, nextWayPointGrid); // convert grid data to real length
    cout << "next way point:" << nextWayPoint[0] << ", " << nextWayPoint[1] << endl;
    
    m_wayPoint.data.position.x = nextWayPoint[0];
    m_wayPoint.data.position.y = nextWayPoint[1];

    m_wayPointOut.write();
    displayStepsMap(stepsMapFromRobot);
  }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t path_plan2::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t path_plan2::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t path_plan2::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t path_plan2::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t path_plan2::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void path_plan2Init(RTC::Manager* manager)
  {
    coil::Properties profile(path_plan2_spec);
    manager->registerFactory(profile,
                             RTC::Create<path_plan2>,
                             RTC::Delete<path_plan2>);
  }
  
};


