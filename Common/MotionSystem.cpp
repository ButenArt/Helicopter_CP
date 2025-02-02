#include "MotionSystem.h"

T_MotionSystem::T_MotionSystem(QObject *parent) : QObject(parent)
{
    AviaMotionSystemActive = false;

    AviaPacketHeader.MagicNumber = 0x7AADF00D;
    AviaPacketHeader.PacketType = 0;
    AviaPacketHeader.DataSize = 0;

    AviaMotionState = new MotionSystemStateDataPacket();
    AviaMotionState->PacketReady = false;
    AviaMotionState->matmodelZ = 0;               // дистанция по оси Z от стартового положения из мат модели (метры)
    AviaMotionState->Roll = 0;                    // крен (градусы -360 .. 360)
    AviaMotionState->Pitch = 0;                   // тангаж (градусы -80 .. 80)
    AviaMotionState->Course = 0;                  // курс (градусы 0 .. 359)

    AviaMotionFeedback = new MotionSystemFeedbackDataPacket();
    AviaMotionFeedback->PacketReady = false;
    AviaMotionFeedback->X = 0;                    // дистанция по оси X от стартового положения из мат модели (метры)
    AviaMotionFeedback->Y = 0;                    // дистанция по оси Y от стартового положения из мат модели (метры)
    AviaMotionFeedback->Z = 0;                    // дистанция по оси Z от стартового положения из мат модели (метры)
    AviaMotionFeedback->Roll = 0;                 // крен (градусы -360 .. 360)
    AviaMotionFeedback->Pitch = 0;                // тангаж (градусы -80 .. 80)
    AviaMotionFeedback->Course = 0;               // курс (градусы 0 .. 359)
}

T_MotionSystem::~T_MotionSystem()
{
    delete AviaMotionState;
    delete AviaMotionFeedback;
}

void T_MotionSystem::Init()
{
    AviaMotionSystemActive = false;

    AviaPacketHeader.MagicNumber = 0x7AADF00D;
    AviaPacketHeader.PacketType = 0;
    AviaPacketHeader.DataSize = 0;

    AviaMotionState->PacketReady = false;
    AviaMotionState->matmodelZ = 0;               // дистанция по оси Z от стартового положения из мат модели (метры)
    AviaMotionState->Roll = 0;                    // крен (градусы -360 .. 360)
    AviaMotionState->Pitch = 0;                   // тангаж (градусы -80 .. 80)
    AviaMotionState->Course = 0;                  // курс (градусы 0 .. 359)

    AviaMotionFeedback->PacketReady = false;
    AviaMotionFeedback->X = 0;                    // дистанция по оси X от стартового положения из мат модели (метры)
    AviaMotionFeedback->Y = 0;                    // дистанция по оси Y от стартового положения из мат модели (метры)
    AviaMotionFeedback->Z = 0;                    // дистанция по оси Z от стартового положения из мат модели (метры)
    AviaMotionFeedback->Roll = 0;                 // крен (градусы -360 .. 360)
    AviaMotionFeedback->Pitch = 0;                // тангаж (градусы -80 .. 80)
    AviaMotionFeedback->Course = 0;               // курс (градусы 0 .. 359)
}

int T_MotionSystem::GetAviaQueueBuffer()
{
    int buffer = 0;
    buffer = AviaMotionStateQueue.count() +
             AviaMotionFeedbackQueue.count();
    return buffer;
}

void T_MotionSystem::ClearAviaQueueBuffer()
{
    AviaMotionStateQueue.clear();
    AviaMotionFeedbackQueue.clear();
}

void T_MotionSystem::AviaMotionStateEnqueuePacket(MotionSystemStateDataPacket packet)
{
    AviaMotionStateQueue.enqueue(packet);
    if(AviaMotionStateQueue.count() > QueueMaxLength)
    {
        AviaMotionStateDequeuePacket();
    }
}

void T_MotionSystem::AviaMotionStateDequeuePacket()
{
    if(!AviaMotionStateQueue.isEmpty()) *AviaMotionState = AviaMotionStateQueue.dequeue();
}

void T_MotionSystem::AviaMotionFeedbackEnqueuePacket(MotionSystemFeedbackDataPacket packet)
{
    AviaMotionFeedbackQueue.enqueue(packet);
    if(AviaMotionFeedbackQueue.count() > QueueMaxLength)
    {
        AviaMotionFeedbackDequeuePacket();
    }
}

void T_MotionSystem::AviaMotionFeedbackDequeuePacket()
{
    if(!AviaMotionFeedbackQueue.isEmpty()) *AviaMotionFeedback = AviaMotionFeedbackQueue.dequeue();
}
