#pragma once
#include <QTime>

#define VAR_CONCAT_INTERNAL(X, Y) X##Y
#define VAR_CONCAT(X, Y) VAR_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME VAR_CONCAT(stvar, __LINE__)

#define PRINT_CHANGED_VAR(_var)  \
{\
static decltype(_var) UNIQUE_VAR_NAME;\
if (UNIQUE_VAR_NAME != _var) {\
	qDebug() << QTime::currentTime() << #_var": " << _var; \
}\
UNIQUE_VAR_NAME = _var;\
}

#define PRINT_CHANGED_VAR2(_var, name)  \
{\
static decltype(_var) UNIQUE_VAR_NAME;\
if (UNIQUE_VAR_NAME != _var) {\
	qDebug() << QTime::currentTime() << #name"("#_var"): " << _var; \
}\
UNIQUE_VAR_NAME = _var;\
}
