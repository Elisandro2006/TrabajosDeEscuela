const readline = require("readline");

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

let gato = [
  ["-", "-", "-"],
  ["-", "-", "-"],
  ["-", "-", "-"]
];


function imprimirPosiciones() {
  let pos = 1;
  console.log("Selecciona una posición en el tablero:");
  console.log("Las casillas marcadas con X u O ya están ocupadas");
  for (let i = 0; i < 3; i++) {
    let fila = "";
    for (let j = 0; j < 3; j++) {
      if (gato[i][j] === "X" || gato[i][j] === "O") {
        fila += " " + gato[i][j];
      } else {
        fila += " " + pos;
      }
      pos++;
    }
    console.log(fila);
  }
}

function casillaNoOcupada(posicion) {
  switch (posicion) {
    case 1: return gato[0][0] === "-";
    case 2: return gato[0][1] === "-";
    case 3: return gato[0][2] === "-";
    case 4: return gato[1][0] === "-";
    case 5: return gato[1][1] === "-";
    case 6: return gato[1][2] === "-";
    case 7: return gato[2][0] === "-";
    case 8: return gato[2][1] === "-";
    case 9: return gato[2][2] === "-";
    default: return false;
  }
}


function registrarJugada(caracter, callback) {
  imprimirPosiciones();
  rl.question(`Jugador ${caracter}, escriba el número de casilla donde desea tirar: `, (entrada) => {
    let posicion = parseInt(entrada);
    if (casillaNoOcupada(posicion)) {
      switch (posicion) {
        case 1: gato[0][0] = caracter; break;
        case 2: gato[0][1] = caracter; break;
        case 3: gato[0][2] = caracter; break;
        case 4: gato[1][0] = caracter; break;
        case 5: gato[1][1] = caracter; break;
        case 6: gato[1][2] = caracter; break;
        case 7: gato[2][0] = caracter; break;
        case 8: gato[2][1] = caracter; break;
        case 9: gato[2][2] = caracter; break;
      }
      callback(true);
    } else {
      console.log("Casilla no válida, escriba una posición válida");
      callback(false);
    }
  });
}


function imprimirGato() {
  console.log("El gato hasta el momento:");
  for (let i = 0; i < 3; i++) {
    console.log("   " + gato[i].join("   "));
  }
}


function ganaPorRenglon(c) {
  for (let i = 0; i < 3; i++) {
    if (gato[i][0] === c && gato[i][1] === c && gato[i][2] === c) {
      return true;
    }
  }
  return false;
}

function ganaPorColumna(c) {
  for (let i = 0; i < 3; i++) {
    if (gato[0][i] === c && gato[1][i] === c && gato[2][i] === c) {
      return true;
    }
  }
  return false;
}

function ganaPorDiagonales(c) {
  if (gato[0][0] === c && gato[1][1] === c && gato[2][2] === c) return true;
  if (gato[0][2] === c && gato[1][1] === c && gato[2][0] === c) return true;
  return false;
}

function hayGanador(c) {
  return ganaPorRenglon(c) || ganaPorColumna(c) || ganaPorDiagonales(c);
}

function hayEspacio() {
  for (let i = 0; i < 3; i++) {
    for (let j = 0; j < 3; j++) {
      if (gato[i][j] === "-") return true;
    }
  }
  return false;
}


function inicializarTablero() {
  gato = [
    ["-", "-", "-"],
    ["-", "-", "-"],
    ["-", "-", "-"]
  ];
}

function main() {
  let jugadorActual = "X";
  let terminar = false;
  inicializarTablero();

  function turno() {
    registrarJugada(jugadorActual, (jugadaValida) => {
      if (jugadaValida) {
        imprimirGato();
        if (hayGanador(jugadorActual)) {
          console.log(`Felicidades jugador ${jugadorActual}, has ganado el juego`);
          terminar = true;
          rl.close();
        } else if (!hayEspacio()) {
          console.log("El juego termina en empate");
          terminar = true;
          rl.close();
        } else {
          jugadorActual = (jugadorActual === "X") ? "O" : "X";
          turno();
        }
      } else {
        turno();
      }
    });
  }

  turno();
}

main();
