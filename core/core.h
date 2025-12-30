#pragma once

extern "C" {
#include"./SDL2/include/SDL.h"
}

#include "managers/managers.h"
#include "utils.h"
#include "core_types.h"
#include "engine.h"
#include "application.h"

/*
* Prosty silnik do gier na potrzeby projektu beat 'em up
* Wykorzystano szablon do projektu oraz bibliotekê SDL2 (2.0.10)
* Sposob korzystania:
* Nalezy zalaczyc plik core.h (#include "core/core.h")
* Oraz rozszerzyc klase Application i zaimplementowac porzadane metody
* 
* Autor: Mikolaj Filipkowski
*/