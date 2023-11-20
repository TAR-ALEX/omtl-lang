#pragma once

#include <exception>

#define tryif(CONDITION) for (bool _once = true; _once;) \
    for (bool _cond = false; _once && (_cond = false, true);) \
        try { _cond = (CONDITION); } \
        catch (...) { _cond = false; } \
        if (_once && _cond) \
            for (_once = false; _once; _once = false)

/*
#define tryif(CONDITION) for (bool _once = true; _once;) \
    for (bool _cond = false; _once && (_cond = false, true);) \
        try { _cond = (CONDITION); } \
        catch (...) { _cond = false; } \
        if (_once && _cond) \
            for (_once = false; _once; _once = false)
            */