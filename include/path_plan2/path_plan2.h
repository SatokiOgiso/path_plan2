// -*- C++ -*-
/*!
 * @file  path_plan2.h
 * @brief ModuleDescription
 * @date  $Date$
 *
 * $Id$
 */

#ifndef PATH_PLAN2_H
#define PATH_PLAN2_H

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">

// </rtc-template>

using namespace RTC;

#define MAPGRIDNUMX 10
#define MAPGRIDNUMY 10
#define MAPDIV 0.3
#define XORG -1.5// origin of robot coordinate grid in real
#define YORG -1.5// origin of robot coordinate grid in real
/*!
 * @class path_plan2
 * @brief ModuleDescription
 *
 */
class path_plan2
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  path_plan2(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~path_plan2();

  // <rtc-template block="public_attribute">
  
  // </rtc-template>

  // <rtc-template block="public_operation">
  
  // </rtc-template>
  inline void makeStepsMap(int zeroLocation[2],int occupiedMap[MAPGRIDNUMX][MAPGRIDNUMY], int stepsMap[MAPGRIDNUMX][MAPGRIDNUMY]);
  inline void displayStepsMap(int stepsMap[MAPGRIDNUMX][MAPGRIDNUMY]);
  inline int determineNextGoal(int nextGoal[2], int stepsMap[MAPGRIDNUMX][MAPGRIDNUMY], int cleanedMap[MAPGRIDNUMX][MAPGRIDNUMY]);
  inline int determineNextWayPoint(int nextWayPoint[2], int stepsMap[MAPGRIDNUMX][MAPGRIDNUMY]);
  inline int realToGrid(double offset[2], int locationGrid[2], double location[2], int mapGridNumX, int mapGridNumY, double mapDivision);
  /***
   *
   * The initialize action (on CREATED->ALIVE transition)
   * formaer rtc_init_entry() 
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onInitialize();

  /***
   *
   * The finalize action (on ALIVE->END transition)
   * formaer rtc_exiting_entry()
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onFinalize();

  /***
   *
   * The startup action when ExecutionContext startup
   * former rtc_starting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStartup(RTC::UniqueId ec_id);

  /***
   *
   * The shutdown action when ExecutionContext stop
   * former rtc_stopping_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onShutdown(RTC::UniqueId ec_id);

  /***
   *
   * The activated action (Active state entry action)
   * former rtc_active_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  /***
   *
   * The deactivated action (Active state exit action)
   * former rtc_active_exit()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  /***
   *
   * The execution action that is invoked periodically
   * former rtc_active_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

  /***
   *
   * The aborting action when main logic error occurred.
   * former rtc_aborting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onAborting(RTC::UniqueId ec_id);

  /***
   *
   * The error action in ERROR state
   * former rtc_error_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onError(RTC::UniqueId ec_id);

  /***
   *
   * The reset action that is invoked resetting
   * This is same but different the former rtc_init_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onReset(RTC::UniqueId ec_id);
  
  /***
   *
   * The state update action that is invoked after onExecute() action
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStateUpdate(RTC::UniqueId ec_id);

  /***
   *
   * The action that is invoked when execution context's rate is changed
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onRateChanged(RTC::UniqueId ec_id);


 protected:
  // <rtc-template block="protected_attribute">
  
  // </rtc-template>

  // <rtc-template block="protected_operation">
  
  // </rtc-template>

  // Configuration variable declaration
  // <rtc-template block="config_declare">
  /*!
   * 
   * - Name:  velocityCoeff
   * - DefaultValue: 1.0
   */
  double m_velocityCoeff;
  /*!
   * 
   * - Name:  angularVelocityCoeff
   * - DefaultValue: 1.0
   */
  double m_angularVelocityCoeff;
  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  TimedPose2D m_robotPose;
  /*!
   */
  InPort<TimedPose2D> m_robotPoseIn;
  TimedDoubleSeq m_obstacleData;
  /*!
   */
  InPort<TimedDoubleSeq> m_obstacleDataIn;
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  TimedPose2D m_wayPoint;
  /*!
   */
  OutPort<TimedPose2D> m_wayPointOut;
  
  // </rtc-template>

  // CORBA Port declaration
  // <rtc-template block="corbaport_declare">
  
  // </rtc-template>

  // Service declaration
  // <rtc-template block="service_declare">
  
  // </rtc-template>

  // Consumer declaration
  // <rtc-template block="consumer_declare">
  
  // </rtc-template>

 private:
  int cleanedMap[MAPGRIDNUMX][MAPGRIDNUMY]; // store the state whether the grid has been cleaned or not.
                                           // 0:not cleaned, 1:cleaned
  double robotLocation[2]; // current robot location (x, y) (m)
  int robotLocationGrid[2];// current robot location on grid (x, y)

  // <rtc-template block="private_attribute">
  
  // </rtc-template>

  // <rtc-template block="private_operation">
  
  // </rtc-template>

};


extern "C"
{
  DLL_EXPORT void path_plan2Init(RTC::Manager* manager);
};

#endif // PATH_PLAN2_H
