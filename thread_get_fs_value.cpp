#include "thread_get_fs_value.h"
#include "FSUIPC_User.h"
#include <QDebug>

thread_get_fs_value::thread_get_fs_value(QObject *parent, bool b) :
    QThread(parent), Stop(b)
{
}




// run() will be called when a thread starts
void thread_get_fs_value::run()
{



    g_function = new global_function();

    //değişkenlern burada olusturursak her seferinde bellek harcamaz
    int chVerticalSpeed;
    int airspeed;
    double heading;
    double pitch; // 0x2F70
    double bank; // 0x2F78
    short turnCoordinator; // 0x037C
    qint8 turnCoordinatorTop ; // 0x036E
    int altimeter; //0x3324
    short altimeter_knob; //0x0330
    double engineRPM; // 0x2400
    short ADFVal; // 0x0C6A
    short ADFKnob; // 0x0C6C
    short VOR2Radial; // 0x0C60
    short VOR2Knob; //0x0C5E
    qint8 VOR2toFrom; // 0x0C5B
    qint8 NAV1GlideSlopeActive; //0x0C4C
    qint8 NAV1ToFrom; //0x0C4B
    qint8 NAV1Localizer; //0x0C48
    qint8 NAV1GlideSlope; //0x0C49
    short NAV1OBS ; // 0x0C4E
    short NAV1Radial; //0x0C50
    int ft_left_level; //0x0B7C
    int ft_right_level; //0x0B94
    short oil_temperature; //0x08B8
    short oil_pressure; // 0x08BA
    double fuelFlow; // 0x0918
    double EGT; // 0x3B70
    double Amper; //0x282C
    double Suction; //0x0B18


     while(1)
    {

         DWORD res;
         DWORD __res = 0 ;





        if(FSUIPC_ReadSpecial(0x02C8,4,&chVerticalSpeed,&res) &&  //vertical speed
                FSUIPC_ReadSpecial(0x02BC,4,&airspeed,&res) &&    //air speed
                FSUIPC_ReadSpecial(0x2B00,8,&heading,&res) &&    //heading
                FSUIPC_ReadSpecial(0x2F70,8,&pitch,&res) &&    //pitch
                FSUIPC_ReadSpecial(0x2F78,8,&bank,&res) &&    //bank
                FSUIPC_ReadSpecial(0x037C,2,&turnCoordinator,&res) &&    //turn coordinator
                FSUIPC_ReadSpecial(0x036E,1,&turnCoordinatorTop,&res) &&    //turn coordinator top
                FSUIPC_ReadSpecial(0x3324,4,&altimeter,&res) &&    //altimeter
                FSUIPC_ReadSpecial(0x0330,4,&altimeter_knob,&res) &&    //altimeter knob ayari
                FSUIPC_ReadSpecial(0x2400,8,&engineRPM,&res) &&    //engine RPM
                FSUIPC_ReadSpecial(0x0C6A,8,&ADFVal,&res) &&    //ADF value değeri
                FSUIPC_ReadSpecial(0x0C6C,8,&ADFKnob,&res) &&    //adf heading Knob ayarı
                FSUIPC_ReadSpecial(0x0C60,2,&VOR2Radial,&res) &&    //Vor2 Radial
                FSUIPC_ReadSpecial(0x0C5E,2,&VOR2Knob,&res) &&    //Vor2 Knob ayarı
                FSUIPC_ReadSpecial(0x0C5B,1,&VOR2toFrom,&res) &&    //Vor2 Knob ayarı
                FSUIPC_ReadSpecial(0x0C4C,1,&NAV1GlideSlopeActive,&res) &&    //Vor2 Knob ayarı
                FSUIPC_ReadSpecial(0x0C4B,1,&NAV1ToFrom,&res) &&    //Vor2 Knob ayarı
                FSUIPC_ReadSpecial(0x0C48,1,&NAV1Localizer,&res) &&    //Vor2 Knob ayarı
                FSUIPC_ReadSpecial(0x0C49,1,&NAV1GlideSlope,&res) &&    //Vor2 Knob ayarı
                FSUIPC_ReadSpecial(0x0C4E,2,&NAV1OBS,&res) &&    //Vor2 Knob ayarı
                FSUIPC_ReadSpecial(0x0C50,2,&NAV1Radial,&res) &&    //Vor2 Knob ayarı
                FSUIPC_ReadSpecial(0x0B7C,4,&ft_left_level,&res) &&    //Left Fuel Tank Level
                FSUIPC_ReadSpecial(0x0B94,4,&ft_right_level,&res) &&    //Right Fuel Tank Level
                FSUIPC_ReadSpecial(0x08B8,2,&oil_temperature,&res) &&    //oil temperatur
                FSUIPC_ReadSpecial(0x08BA,2,&oil_pressure,&res) &&    //oil pressure
                FSUIPC_ReadSpecial(0x0918,8,&fuelFlow,&res) &&    //FuelFlow
                FSUIPC_ReadSpecial(0x3B70,8,&EGT,&res) &&    //EGT
                FSUIPC_ReadSpecial(0x282C,8,&Amper,&res) &&    //Amper
                FSUIPC_ReadSpecial(0x0B18,8,&Suction,&res)     //Suction

               && FSUIPC_Process(&__res) )  // FSUIPC_Process(&res)
        {


      //      qDebug( QString::number(altimeter_knob).toLatin1() );

            emit have_fsVal(
                        airspeed / 128.0,
                        chVerticalSpeed * 60 * 3.28084 / 256,
                        heading,
                        pitch * (-1),
                        bank * (-1),
                        turnCoordinator,
                        turnCoordinatorTop,
                        altimeter ,
                        altimeter_knob * 29.92 / 16210,
                        engineRPM,
                        ADFVal  * 360 / 65536 ,
                        ADFKnob ,
                        VOR2Radial * 360 / 65536 ,
                        VOR2Knob ,
                        VOR2toFrom  ,
                        NAV1GlideSlopeActive ,
                        NAV1ToFrom ,
                        NAV1Localizer ,
                        NAV1GlideSlope ,
                        NAV1OBS ,
                        NAV1Radial *360 / 65536,
                        ft_left_level * 100 /8388608,
                        ft_right_level * 100 / 8388608,
                        ( oil_temperature * 140 / 16384 ) * 1.8 + 32 ,  // ikinci kısım fahrenayta döndermek için ilki cantigrat olarak depeğer verir
                        oil_pressure * 55 / 16384,
                        fuelFlow,
                        EGT,
                        Amper,
                        Suction

                    );


        }



        this->msleep(1);
    }
}
