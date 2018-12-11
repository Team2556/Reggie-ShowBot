#include <iostream>
#include <memory>
#include <string>
#include "WPILib.h"
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include "RobotDrive.h"
#include "XboxController.h"
#include "ctre/Phoenix.h"
#include "MotorSafetyHelper.h"
#include <DoubleSolenoid.h>
#include "timer.h"
#include "DigitalInput.h"
#include "math.h"
#include <Preferences.h>
#include "Robot.h"
#include <iostream>
#include <Servo.h>
#include <SmartDashboard/SmartDashboard.h>

#include <NavGyro.h>
#include <RobotMap.h>



	cs::UsbCamera		UsbCamera1;


	XboxController* 	pclXbox;

	WPI_TalonSRX *lf  = new WPI_TalonSRX(CAN_TALON_LEFT_FRONT);   /* left front     */
	WPI_TalonSRX *lr  = new WPI_TalonSRX(CAN_TALON_LEFT_REAR);    /* left rear      */
	WPI_TalonSRX *rf  = new WPI_TalonSRX(CAN_TALON_RIGHT_FRONT);  /* right front    */
	WPI_TalonSRX *rr  = new WPI_TalonSRX(CAN_TALON_RIGHT_REAR);   /* right rear     */
	WPI_TalonSRX *ss  = new WPI_TalonSRX(CAN_TALON_SS_RAISE);

	MecanumDrive *m_robotDrive;
	NavGyro * pNavGyro;
	Relay * pCannon;

	DoubleSolenoid *	climbSolenoid;

	Servo* fog;






void Robot::RobotInit() {
	lf->Set(ControlMode::PercentOutput, 0);
	lr->Set(ControlMode::PercentOutput, 0);
	rf->Set(ControlMode::PercentOutput, 0);
	rr->Set(ControlMode::PercentOutput, 0);
	ss->Set(ControlMode::PercentOutput, 0);

	 m_robotDrive = new MecanumDrive(*lf, *lr, *rf, *rr);

	 pclXbox  = new XboxController(0);

	 pNavGyro = new NavGyro();

	 pCannon = new Relay(RELAY_CHAN_CANNON, frc::Relay::kReverseOnly);

	 climbSolenoid = new DoubleSolenoid(CAN_PCM, 0, 1);

	 fog = new Servo(0);

	 UsbCamera1 = CameraServer::GetInstance()->StartAutomaticCapture();
	 UsbCamera1.SetResolution(700, 600);
	 UsbCamera1.SetFPS(30);
}

/**
 * This autonomous (along with the chooser code above) shows how to select
 * between different autonomous modes using the dashboard. The sendable chooser
 * code works with the Java SmartDashboard. If you prefer the LabVIEW Dashboard,
 * remove all of the chooser code and uncomment the GetString line to get the
 * auto name from the text box below the Gyro.
 *
 * You can add additional auto modes by adding additional comparisons to the
 * if-else structure below with additional strings. If using the SendableChooser
 * make sure to add them to the chooser code above as well.
 */
int ctr;
void Robot::AutonomousInit()
{
	ctr = 0;
}

void Robot::AutonomousPeriodic()
{
	double timer = DriverStation::GetInstance().GetMatchTime();
	if (timer >= 2 and timer < 12)
	{
		ctr++;
	}
	SmartDashboard::PutNumber("Counter", ctr);

	m_robotDrive->DriveCartesian(1,-1,0,0);
}

void Robot::TeleopInit()
{

}

void Robot::TeleopPeriodic()
{
	double fXStick = pclXbox->GetX(frc::XboxController::kLeftHand)*0.7;
	double fYStick = (pclXbox->GetY(frc::XboxController::kLeftHand) * -1.0)*0.7;
	double fRotate = pclXbox->GetX(frc::XboxController::kRightHand)*0.7;

	if(pclXbox->GetPOV()>-1 && pNavGyro->bPresetTurning == false)
	{
		pNavGyro->fGyroCommandYaw = pNavGyro->fGyroCommandYaw + pclXbox->GetPOV();
		pNavGyro->bPresetTurning = true;
	}
	if(fabs(pNavGyro->GetYawError())<10)
	{
		pNavGyro->bPresetTurning = false;
	}
	// Handle manual rotation
	bool bAllowRotate = pclXbox->GetTriggerAxis(frc::XboxController::kRightHand)>.5;
	//bAllowRotate = pclXbox->GetX(frc::XboxController::kRightHand)>.1||
		//		   pclXbox->GetX(frc::XboxController::kRightHand)<-.1;
	if (!bAllowRotate)
	{
	fRotate = pclXbox->GetX(frc::XboxController::kRightHand);
	fRotate = pNavGyro->CorrectRotate(fRotate);
	pNavGyro->SetCommandYawToCurrent();
	}
	else
	{
		// Calculate a rotation rate from robot angle error
		fRotate = pNavGyro->GetRotate();
	}

	m_robotDrive->DriveCartesian(fXStick, fYStick, fRotate, 0.0);

	if(pclXbox->GetBumper(frc::XboxController::kLeftHand)>0 && pclXbox->GetBumper(frc::XboxController::kRightHand)>0)
	{
		pCannon->Set(pclXbox->GetAButton() ? frc::Relay::Value::kReverse : frc::Relay::Value::kOff);
	}

	ss->Set(pclXbox->GetY(frc::XboxController::kRightHand));

	climbSolenoid->Set(frc::DoubleSolenoid::Value::kOff);

	SmartDashboard::PutNumber("ANgle", fog->Get());
	if (pclXbox->GetBButton())
	{
		fog->SetAngle(30);
	}
	else
	{
		fog->SetAngle(50);
	}


}

void Robot::TestPeriodic() {}


START_ROBOT_CLASS(Robot)
