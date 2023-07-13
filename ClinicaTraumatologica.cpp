binarysem sem_medico = 0;
binarysem sem_escayola = 0;
binarysem sem_xray = 0;
binarysem sem_nurse = 1;

/// LLENAR LAS VARIABLES CON LA CANTIDAD DE PACIENTES PARA EL MODELO ///////////

int paciente_rayos = 100, paciente_medico = 100, paciente_escayola = 100;

////////////////////////////////////////////////////////////////////////////////

int sala_medico = 0, sala_escayola = 0;

int total_espera = 0;

void entradasalamedico(){

    if (paciente_medico > 20) {
        sala_medico = 20;
        paciente_medico = paciente_medico - 20;
    }
    else {
        sala_medico = paciente_medico;
    }

}

void entradasalaescayola(){

    if (paciente_escayola > 6) {
        sala_escayola = 6;
        paciente_escayola = paciente_escayola - 6;
    }
    else {
        sala_escayola = paciente_escayola;
    }

}

void salidasalamedico(){

    if(paciente_medico > 20){
        sala_medico = sala_medico + 1;
    }
    else{
        sala_medico = paciente_medico;
    }

}

void salidasalaescayola(){

    if (paciente_escayola > 6 ){
        sala_escayola = sala_escayola + 1;
    }
    else{
        sala_escayola = paciente_escayola;
    }

}

void xray() {
    while (paciente_rayos > 0) {
        wait(sem_xray);

        paciente_rayos = paciente_rayos - 1;

        signal(sem_xray);
    }
    cout << "Todos los X-Rays están listos" << endl;
}

void medico() {
    entradasalamedico();
    
    while (paciente_medico > 0) {
        wait(sem_medico);

        sala_medico = sala_medico - 1;

        paciente_medico = paciente_medico - 1;
        
        salidasalamedico();
        
        signal(sem_medico);
    }
    cout << "Todas las atenciones médicas están listas" << endl;
    
}

void escayola() {
    entradasalaescayola();
    while (paciente_escayola > 0) {
        wait(sem_escayola);


        sala_escayola = sala_escayola - 1;

        paciente_escayola = paciente_escayola - 1;
        salidasalaescayola();
        signal(sem_escayola);
  
    }
    cout << "Todas las escayolas están listas" << endl;
   
}

void nurse() {
    wait(sem_nurse);
    total_espera = paciente_medico + paciente_escayola + paciente_rayos;

    while (total_espera > 0) {
        cout << "Pacientes esperando ser atendidos: " << total_espera << endl;

        signal(sem_medico);
        wait(sem_medico);
        signal(sem_escayola);
        wait(sem_escayola);
        signal(sem_xray);
        wait(sem_xray);
        total_espera = paciente_medico + paciente_escayola + paciente_rayos;

    }
}

void main() {
    
    cobegin {
        nurse();
        medico();
        escayola();
        xray();
    }

    cout << "Todos los pacientes han sido atendidos " << endl;

    
}