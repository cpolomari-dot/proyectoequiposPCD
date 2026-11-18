
set(NOMBRE_EJECUTABLE banco)

add_executable(${NOMBRE_EJECUTABLE}
    main.cpp

    CuentaBancaria.h
    CuentaBancaria.cpp

    Cliente.h
    Cliente.cpp

    Banco.h
    Banco.cpp
)

target_link_libraries(${NOMBRE_EJECUTABLE} PRIVATE base semaforo)
