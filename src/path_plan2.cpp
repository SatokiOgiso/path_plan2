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

#define DEBUG_
#define DEBUG_MAP  // show map
#define DEBUG_CMAP // make cleaned map manually
#define DEBUG_OMAP // make occupied map manually

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
  robotLocationGrid[0] = 0;
  robotLocationGrid[1] = 0;

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

inline int path_plan2::determineNextWayPoint(int nextWayPoint[2], int stepsMap[MAPGRIDNUMX][MAPGRIDNUMY])
{
  int goalValid = 0;

  return goalValid;
}

inline int path_plan2::realToGrid(double offset[2], int locationGrid[2], double location[2])
{
  int gridValid = 1;

  cout << "real:" << location[0] - offset[0] << ", " << location[1] - offset[1] << endl;

  locationGrid[0] = (int)((location[0] - XORG)/MAPDIV); // calculate grid number for x
  locationGrid[1] = (int)((location[1] - YORG)/MAPDIV); // calculate grid number for y 

  if(MAPGRIDNUMX <= locationGrid[0] || MAPGRIDNUMY <= locationGrid[1] || 0 > locationGrid[0] || 0 > locationGrid[1]){
    gridValid = 0; // the calculated grid is out of map
  }

  return gridValid;
}


RTC::ReturnCode_t path_plan2::onExecute(RTC::UniqueId ec_id)
{
  double gridOrigin[2] = {XORG, YORG};
  int robotGridValid = 0;
  int nextGoal[2] = {0, 0}; // next goal on grid (x, y)
  double nextWayPoint[2] = {0, 0}; // next way point (x, y)(m)
  int nextWayPointGrid[2] = {0, 0}; //next way point on grid (x, y)
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
    robotGridValid = realToGrid(gridOrigin, robotLocationGrid, robotLocation, MAPGRIDNUMX, MAPGRIDNUMY, MAPDIV); 
    cleanedMap[robotLocationGrid[0]][robotLocationGrid[1]] = 1; // set cleaned on cleaned map
    cout << "grid:" << robotLocationGrid[0]<< ", " << robotLocationGrid[1] << endl; // display current grids
  }

#ifdef DEBUG_
  robotLocation[0] = 0.3; // store current robot x location 
  robotLocation[1] = 1.2; // store current robot y location
  robotGridValid = realToGrid(gridOrigin, robotLocationGrid, robotLocation, MAPGRIDNUMX, MAPGRIDNUMY, MAPDIV);
  cleanedMap[robotLocationGrid[0]][robotLocationGrid[1]] = 1; // set cleaned on cleaned map
  cout << "grid:" << robotLocationGrid[0] << ", " << robotLocationGrid[1] << endl; // display current grids
#endif


#ifdef DEBUG_CMAP
  cleanedMap[0][0] = 1;
  cleanedMap[0][1] = 1;
  cleanedMap[1][0] = 1;
  cleanedMap[1][1] = 1;
#endif


  // make occupied map
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
 
  makeStepsMap(robotLocationGrid, occupiedMap, stepsMapFromRobot);// make steps map from robot

  // determine next goal from steps map from robot & cleaned map
  determineNextGoal(nextGoal, stepsMapFromRobot, cleanedMap); // make next goal from robot

#ifdef DEBUG_
  cout << "next goal:" << nextGoal[0] << ", " << nextGoal[1] << endl;
#endif

  // make steps map from goal
  makeStepsMap(nextGoal, occupiedMap, stepsMapFromGoal);

  // determine next way point
  determineNextWayPoint(nextWayPointGrid, stepsMapFromGoal);


#ifdef DEBUG_MAP
  displayStepsMap(stepsMapFromRobot);
#endif

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


