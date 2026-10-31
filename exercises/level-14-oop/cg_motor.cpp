#include "cg_motor.hpp"
#include <string.h>

extern "C" {
#include "cg_cmd.h"
}

CyberGearMotor::CyberGearMotor(CanTransport &bus, uint8_t hostId, uint8_t motorId)
    : bus_(bus), host_(hostId), id_(motorId), updated_(false)
{
    /* TODO: nolkan fb_ (memset). Daftar inisialisasi di atas sudah mengisi bus_, host_, id_, updated_. */
}

bool CyberGearMotor::enable()
{
    /* TODO: CgFrame f; cg_build_enable(&f, host_, id_); return bus_.send(f); */
    return false;
}

bool CyberGearMotor::disable()    { /* TODO */ return false; }
bool CyberGearMotor::clearFault() { /* TODO */ return false; }
bool CyberGearMotor::setZero()    { /* TODO */ return false; }

bool CyberGearMotor::setRunMode(uint8_t mode)
{
    /* TODO */
    (void)mode;
    return false;
}

bool CyberGearMotor::sendMotion(float pos, float vel, float torque, float kp, float kd)
{
    /* TODO: perhatikan cg_build_motion tidak butuh host */
    (void)pos; (void)vel; (void)torque; (void)kp; (void)kd;
    return false;
}

bool CyberGearMotor::onFrame(const CgFrame &f)
{
    /* TODO:
     *   - decode ke CgFeedback sementara; jika gagal -> return false
     *   - jika motor_id hasil decode != id_  -> return false (frame milik motor lain, jangan ubah state)
     *   - simpan ke fb_, updated_ = true, return true */
    (void)f;
    return false;
}

uint8_t CyberGearMotor::id() const          { /* TODO */ return 0; }
float   CyberGearMotor::position() const    { /* TODO */ return 0; }
float   CyberGearMotor::velocity() const    { /* TODO */ return 0; }
float   CyberGearMotor::torque() const      { /* TODO */ return 0; }
float   CyberGearMotor::temperature() const { /* TODO */ return 0; }
uint8_t CyberGearMotor::mode() const        { /* TODO */ return 0; }
bool    CyberGearMotor::hasFault() const    { /* TODO: fault_bits != 0 */ return false; }
bool    CyberGearMotor::isUpdated() const   { /* TODO */ return false; }
void    CyberGearMotor::clearUpdated()      { /* TODO */ }
