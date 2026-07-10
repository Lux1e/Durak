#pragma once


enum class SendResult
{
    Done,
    Partial,
    NotReady,
    Disconnected,
    Error
};