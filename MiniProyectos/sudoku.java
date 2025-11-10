import java.util.*;
import java.io.*;

public class sudoku {
    private static final int TAM = 9;
    private int[][] tablero = new int[TAM][TAM];
    private int[][] solucion = new int[TAM][TAM];
    private boolean[][] fijo = new boolean[TAM][TAM];
    private Random rand = new Random();
    private int nivelActual = 1;
    private int subnivelActual = 1;
    private int vidas = 3;
    private int vidasMaximas = 3;
    private int puntuacion = 0;
    private int movimientos = 0;
    private long tiempoInicio = 0;
    private int pistasUsadas = 0;

    public static void main(String[] args) {
        sudoku juego = new sudoku();
        juego.ejecutar();
    }

    private void ejecutar() {
        Scanner sc = new Scanner(System.in);
        limpiarPantalla();
        System.out.println("=== SUDOKU EN CONSOLA ===");
        System.out.println("Comienza un nuevo juego con 3 niveles de dificultad.");
        System.out.println("Cada nivel tiene 5 subniveles (total: 15 niveles)");
        
        System.out.print("\n¿Cargar partida guardada? (s/n): ");
        String cargar = sc.nextLine().trim().toLowerCase();
        
        if (cargar.startsWith("s") && cargarPartida()) {
            System.out.println("Partida cargada exitosamente!");
            esperarEnter(sc);
        } else {
            elegirNivelCompleto(sc);
            nuevoJuego();
        }

        while (true) {
            limpiarPantalla();
            mostrarTablero();
            mostrarEstadisticas();
            System.out.println("""
Comandos disponibles:
poner f c v   -> coloca un numero (fila, columna, valor)
borrar f c    -> borra una celda
pista         -> muestra un numero correcto al azar (-50 puntos)
comprobar     -> verifica si el tablero esta completo y correcto
guardar       -> guarda la partida actual
nuevo         -> genera un nuevo tablero
nivel         -> cambia el nivel de dificultad
subir         -> aumenta la dificultad (subnivel + 1)
resolver      -> muestra la solucion (termina el juego)
estadisticas  -> muestra estadisticas detalladas
salir         -> termina el juego
""");
            System.out.print("> ");
            String linea = sc.nextLine().trim();
            if (linea.isEmpty()) continue;
            String[] partes = linea.split("\\s+");
            String cmd = partes[0].toLowerCase();

            try {
                switch (cmd) {
                    case "poner":
                        if (partes.length != 4) { 
                            System.out.println("Uso correcto: poner fila columna valor"); 
                            break; 
                        }
                        int f = Integer.parseInt(partes[1]) - 1;
                        int c = Integer.parseInt(partes[2]) - 1;
                        int v = Integer.parseInt(partes[3]);
                        ponerCelda(f, c, v);
                        break;

                    case "borrar":
                        if (partes.length != 3) { 
                            System.out.println("Uso correcto: borrar fila columna"); 
                            break; 
                        }
                        borrarCelda(Integer.parseInt(partes[1]) - 1, Integer.parseInt(partes[2]) - 1);
                        break;

                    case "pista":
                        darPista();
                        break;

                    case "comprobar":
                        if (completo()) {
                            if (resuelto()) {
                                limpiarPantalla();
                                mostrarTablero();
                                calcularPuntuacionFinal();
                                mostrarResumenFinal();
                                System.out.print("\n¿Quieres jugar otro? (s/n): ");
                                if (sc.nextLine().trim().toLowerCase().startsWith("s")) {
                                    elegirNivelCompleto(sc);
                                    nuevoJuego();
                                } else return;
                            } else {
                                System.out.println("El tablero esta completo, pero hay errores.");
                            }
                        } else {
                            System.out.println("Aun no has completado el tablero.");
                        }
                        break;

                    case "guardar":
                        if (guardarPartida()) {
                            System.out.println("Partida guardada exitosamente!");
                        } else {
                            System.out.println("Error al guardar la partida.");
                        }
                        esperarEnter(sc);
                        break;

                    case "nuevo":
                        nuevoJuego();
                        break;

                    case "nivel":
                        elegirNivelCompleto(sc);
                        nuevoJuego();
                        break;

                    case "subir":
                        if (subnivelActual < 5) {
                            subnivelActual++;
                        } else if (nivelActual < 3) {
                            nivelActual++;
                            subnivelActual = 1;
                        } else {
                            System.out.println("Ya estas en el nivel maximo de dificultad (Dificil 5).");
                            break;
                        }
                        System.out.println("Nivel aumentado a: " + obtenerNombreNivelCompleto());
                        nuevoJuego();
                        break;

                    case "estadisticas":
                        mostrarEstadisticasDetalladas();
                        esperarEnter(sc);
                        break;

                    case "resolver":
                        mostrarSolucion();
                        esperarEnter(sc);
                        break;

                    case "salir":
                        System.out.print("¿Guardar partida antes de salir? (s/n): ");
                        if (sc.nextLine().trim().toLowerCase().startsWith("s")) {
                            guardarPartida();
                        }
                        System.out.println("Saliendo del juego. Hasta pronto!");
                        return;

                    default:
                        System.out.println("Comando desconocido. Escribe uno de los mostrados arriba.");
                }
            } catch (NumberFormatException ex) {
                System.out.println("Entrada numerica invalida. Usa solo numeros del 1 al 9.");
            } catch (IndexOutOfBoundsException ex) {
                System.out.println("Coordenadas fuera de rango. Usa valores entre 1 y 9.");
            }
        }
    }

    private void elegirNivelCompleto(Scanner sc) {
        while (true) {
            System.out.print("Elige nivel (1 = facil, 2 = medio, 3 = dificil): ");
            String s = sc.nextLine().trim();
            if (s.equals("1") || s.equalsIgnoreCase("f")) {
                nivelActual = 1;
                break;
            }
            if (s.equals("2") || s.equalsIgnoreCase("m")) {
                nivelActual = 2;
                break;
            }
            if (s.equals("3") || s.equalsIgnoreCase("d")) {
                nivelActual = 3;
                break;
            }
            System.out.println("Nivel invalido. Intenta 1, 2 o 3.");
        }
        
        while (true) {
            System.out.print("Elige subnivel (1-5): ");
            String s = sc.nextLine().trim();
            try {
                int subnivel = Integer.parseInt(s);
                if (subnivel >= 1 && subnivel <= 5) {
                    subnivelActual = subnivel;
                    break;
                }
            } catch (NumberFormatException e) {
                // continuar el bucle
            }
            System.out.println("Subnivel invalido. Intenta un numero del 1 al 5.");
        }
    }

    private String obtenerNombreNivel() {
        return switch (nivelActual) {
            case 1 -> "Facil";
            case 2 -> "Medio";
            case 3 -> "Dificil";
            default -> "Medio";
        };
    }

    private String obtenerNombreNivelCompleto() {
        return obtenerNombreNivel() + " " + subnivelActual;
    }

    private void mostrarEstadisticas() {
        System.out.println("Nivel actual: " + obtenerNombreNivelCompleto() + 
                         " (" + calcularNivelGlobal() + "/15)");
        System.out.print("Vidas: ");
        for (int i = 0; i < vidasMaximas; i++) {
            if (i < vidas) {
                System.out.print("# ");
            } else {
                System.out.print("- ");
            }
        }
        System.out.println(" | Puntos: " + puntuacion + " | Movimientos: " + movimientos);
    }

    private void mostrarEstadisticasDetalladas() {
        limpiarPantalla();
        System.out.println("=" .repeat(60));
        System.out.println("              ESTADISTICAS DETALLADAS");
        System.out.println("=" .repeat(60));
        System.out.println("Nivel actual: " + obtenerNombreNivelCompleto());
        System.out.println("Nivel global: " + calcularNivelGlobal() + "/15");
        System.out.println("Vidas restantes: " + vidas + "/" + vidasMaximas);
        System.out.println("Puntuacion actual: " + puntuacion);
        System.out.println("Movimientos realizados: " + movimientos);
        System.out.println("Pistas usadas: " + pistasUsadas);
        System.out.println("Tiempo de juego: " + obtenerTiempoTranscurrido());
        System.out.println("Celdas completadas: " + contarCeldasCompletas() + "/81");
        System.out.println("=" .repeat(60));
    }

    private int calcularNivelGlobal() {
        return (nivelActual - 1) * 5 + subnivelActual;
    }

    private int calcularCeldasAEliminar() {
        int base = switch (nivelActual) {
            case 1 -> 35; // Facil
            case 2 -> 40; // Medio
            case 3 -> 45; // Dificil
            default -> 40;
        };
        return base + (subnivelActual - 1) * 5;
    }

    private void nuevoJuego() {
        generarSolucionCompleta();
        copiar(solucion, tablero);
        int quitar = calcularCeldasAEliminar();
        eliminarCeldas(quitar);
        marcarFijas();
        vidas = vidasMaximas;
        puntuacion = 0;
        movimientos = 0;
        pistasUsadas = 0;
        tiempoInicio = System.currentTimeMillis();
        System.out.println("Nuevo juego iniciado (" + obtenerNombreNivelCompleto() + 
                         ", " + quitar + " celdas vacias). Buena suerte!");
    }

    private void generarSolucionCompleta() {
        int[][] grid = new int[TAM][TAM];
        llenarGrid(grid, 0, 0);
        solucion = grid;
    }

    private boolean llenarGrid(int[][] g, int fila, int col) {
        if (fila == TAM) return true;
        int sigFila = (col == TAM - 1) ? fila + 1 : fila;
        int sigCol = (col == TAM - 1) ? 0 : col + 1;

        List<Integer> nums = new ArrayList<>();
        for (int i = 1; i <= TAM; i++) nums.add(i);
        Collections.shuffle(nums, rand);

        for (int num : nums) {
            if (puedeColocar(g, fila, col, num)) {
                g[fila][col] = num;
                if (llenarGrid(g, sigFila, sigCol)) return true;
                g[fila][col] = 0;
            }
        }
        return false;
    }

    private boolean puedeColocar(int[][] g, int f, int c, int val) {
        for (int i = 0; i < TAM; i++) if (g[f][i] == val || g[i][c] == val) return false;
        int bf = (f / 3) * 3, bc = (c / 3) * 3;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (g[bf + i][bc + j] == val) return false;
        return true;
    }

    private void eliminarCeldas(int cantidad) {
        List<Integer> pos = new ArrayList<>();
        for (int i = 0; i < TAM * TAM; i++) pos.add(i);
        Collections.shuffle(pos, rand);
        for (int i = 0; i < cantidad && i < pos.size(); i++) {
            int p = pos.get(i);
            int f = p / TAM, c = p % TAM;
            tablero[f][c] = 0;
        }
    }

    private void marcarFijas() {
        for (int i = 0; i < TAM; i++)
            for (int j = 0; j < TAM; j++)
                fijo[i][j] = tablero[i][j] != 0;
    }

    private void mostrarTablero() {
        System.out.println("+-------+-------+-------+");
        for (int i = 0; i < TAM; i++) {
            System.out.print("| ");
            for (int j = 0; j < TAM; j++) {
                if (tablero[i][j] == 0) System.out.print(". ");
                else System.out.print(tablero[i][j] + " ");
                if ((j + 1) % 3 == 0) System.out.print("| ");
            }
            System.out.println();
            if ((i + 1) % 3 == 0) System.out.println("+-------+-------+-------+");
        }
    }

    private void ponerCelda(int f, int c, int v) {
        validarCoords(f, c);
        if (fijo[f][c]) { 
            System.out.println("Esa celda es fija, no se puede modificar."); 
            return; 
        }
        if (v < 1 || v > 9) { 
            System.out.println("Valor invalido. Usa numeros del 1 al 9."); 
            return; 
        }
        if (!movimientoValido(f, c, v)) { 
            System.out.println("Movimiento invalido: rompe las reglas de fila, columna o subcuadro.");
            perderVida();
            return; 
        }
        
        movimientos++;
        
        if (v != solucion[f][c]) {
            System.out.println("Numero incorrecto! El numero correcto no es " + v);
            perderVida();
            return;
        }
        
        tablero[f][c] = v;
        int puntosGanados = calcularPuntosPorCelda();
        puntuacion += puntosGanados;
        System.out.println("Correcto! (+" + puntosGanados + " puntos)");
    }

    private int calcularPuntosPorCelda() {
        // Puntos base según nivel
        int base = switch (nivelActual) {
            case 1 -> 10;
            case 2 -> 20;
            case 3 -> 30;
            default -> 10;
        };
        // Bonus por subnivel
        int bonus = subnivelActual * 5;
        return base + bonus;
    }

    private void calcularPuntuacionFinal() {
        // Bonus por vidas restantes
        int bonusVidas = vidas * 100;
        
        // Bonus por tiempo (si resolvio rapido)
        long tiempoSegundos = (System.currentTimeMillis() - tiempoInicio) / 1000;
        int bonusTiempo = Math.max(0, (int)(1000 - tiempoSegundos));
        
        // Bonus por nivel
        int bonusNivel = calcularNivelGlobal() * 50;
        
        // Penalizacion por movimientos excesivos
        int celdasVacias = calcularCeldasAEliminar();
        int penalizacion = Math.max(0, (movimientos - celdasVacias) * 5);
        
        puntuacion += bonusVidas + bonusTiempo + bonusNivel - penalizacion;
        
        System.out.println("\n" + "=".repeat(60));
        System.out.println("           FELICIDADES! SUDOKU RESUELTO!");
        System.out.println("=".repeat(60));
    }

    private void mostrarResumenFinal() {
        System.out.println("Nivel completado: " + obtenerNombreNivelCompleto());
        System.out.println("Tiempo total: " + obtenerTiempoTranscurrido());
        System.out.println("Movimientos: " + movimientos);
        System.out.println("Pistas usadas: " + pistasUsadas);
        System.out.println("Vidas restantes: " + vidas + "/" + vidasMaximas);
        System.out.println("-".repeat(60));
        System.out.println("PUNTUACION FINAL: " + puntuacion + " puntos");
        System.out.println("=".repeat(60));
    }

    private void borrarCelda(int f, int c) {
        validarCoords(f, c);
        if (fijo[f][c]) { 
            System.out.println("Esa celda es fija, no se puede borrar."); 
            return; 
        }
        tablero[f][c] = 0;
        movimientos++;
    }

    private void validarCoords(int f, int c) {
        if (f < 0 || f >= TAM || c < 0 || c >= TAM)
            throw new IndexOutOfBoundsException();
    }

    private boolean movimientoValido(int f, int c, int v) {
        for (int i = 0; i < TAM; i++) if (tablero[f][i] == v) return false;
        for (int i = 0; i < TAM; i++) if (tablero[i][c] == v) return false;
        int bf = (f / 3) * 3, bc = (c / 3) * 3;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (tablero[bf + i][bc + j] == v) return false;
        return true;
    }

    private void darPista() {
        List<int[]> vacias = new ArrayList<>();
        for (int i = 0; i < TAM; i++)
            for (int j = 0; j < TAM; j++)
                if (tablero[i][j] == 0)
                    vacias.add(new int[]{i, j});
        if (vacias.isEmpty()) {
            System.out.println("No hay celdas vacias para dar una pista.");
            return;
        }
        int[] elegida = vacias.get(rand.nextInt(vacias.size()));
        tablero[elegida[0]][elegida[1]] = solucion[elegida[0]][elegida[1]];
        puntuacion = Math.max(0, puntuacion - 50);
        pistasUsadas++;
        System.out.printf("Pista: celda (%d,%d) completada. (-50 puntos)\n", elegida[0] + 1, elegida[1] + 1);
    }

    private void mostrarSolucion() {
        copiar(solucion, tablero);
        puntuacion = 0;
        System.out.println("Solucion revelada. La partida ha terminado sin puntos.");
    }

    private boolean completo() {
        for (int i = 0; i < TAM; i++)
            for (int j = 0; j < TAM; j++)
                if (tablero[i][j] == 0) return false;
        return true;
    }

    private boolean resuelto() {
        for (int i = 0; i < TAM; i++)
            for (int j = 0; j < TAM; j++)
                if (tablero[i][j] != solucion[i][j]) return false;
        return true;
    }

    private int contarCeldasCompletas() {
        int count = 0;
        for (int i = 0; i < TAM; i++)
            for (int j = 0; j < TAM; j++)
                if (tablero[i][j] != 0) count++;
        return count;
    }

    private String obtenerTiempoTranscurrido() {
        long segundos = (System.currentTimeMillis() - tiempoInicio) / 1000;
        long minutos = segundos / 60;
        segundos = segundos % 60;
        return String.format("%d min %d seg", minutos, segundos);
    }

    private void copiar(int[][] src, int[][] dst) {
        for (int i = 0; i < TAM; i++)
            for (int j = 0; j < TAM; j++)
                dst[i][j] = src[i][j];
    }

    private void limpiarPantalla() {
        try {
            String os = System.getProperty("os.name");
            if (os.contains("Windows")) {
                new ProcessBuilder("cmd", "/c", "cls").inheritIO().start().waitFor();
            } else {
                System.out.print("\033[H\033[2J");
                System.out.flush();
            }
        } catch (Exception e) {
            for (int i = 0; i < 50; i++) {
                System.out.println();
            }
        }
    }

    private void esperarEnter(Scanner sc) {
        System.out.print("\nPresiona Enter para continuar...");
        sc.nextLine();
    }

    private void perderVida() {
        vidas--;
        System.out.println("Perdiste una vida. Vidas restantes: " + vidas);
        
        if (vidas <= 0) {
            System.out.println("\nGame Over! Te has quedado sin vidas.");
            System.out.println("Puntuacion final: " + puntuacion);
            System.out.println("La solucion era:");
            mostrarSolucion();
            
            Scanner sc = new Scanner(System.in);
            System.out.print("\n¿Quieres continuar con un nuevo juego? (s/n): ");
            String respuesta = sc.nextLine().trim().toLowerCase();
            
            if (respuesta.startsWith("s")) {
                System.out.print("¿Mismo nivel o elegir otro? (m/e): ");
                String opcion = sc.nextLine().trim().toLowerCase();
                
                if (opcion.startsWith("e")) {
                    elegirNivelCompleto(sc);
                }
                nuevoJuego();
            } else {
                System.out.println("Gracias por jugar! Hasta pronto.");
                System.exit(0);
            }
        }
    }

    // ==================== GUARDAR Y CARGAR PARTIDA ====================
    
    private boolean guardarPartida() {
        try (PrintWriter writer = new PrintWriter(new FileWriter("sudoku_save.txt"))) {
            writer.println(nivelActual);
            writer.println(subnivelActual);
            writer.println(vidas);
            writer.println(puntuacion);
            writer.println(movimientos);
            writer.println(pistasUsadas);
            writer.println(tiempoInicio);
            
            // Guardar tablero
            for (int i = 0; i < TAM; i++) {
                for (int j = 0; j < TAM; j++) {
                    writer.print(tablero[i][j] + " ");
                }
                writer.println();
            }
            
            // Guardar solucion
            for (int i = 0; i < TAM; i++) {
                for (int j = 0; j < TAM; j++) {
                    writer.print(solucion[i][j] + " ");
                }
                writer.println();
            }
            
            // Guardar celdas fijas
            for (int i = 0; i < TAM; i++) {
                for (int j = 0; j < TAM; j++) {
                    writer.print((fijo[i][j] ? "1" : "0") + " ");
                }
                writer.println();
            }
            
            return true;
        } catch (IOException e) {
            return false;
        }
    }
    
    private boolean cargarPartida() {
        try (Scanner scanner = new Scanner(new File("sudoku_save.txt"))) {
            nivelActual = scanner.nextInt();
            subnivelActual = scanner.nextInt();
            vidas = scanner.nextInt();
            puntuacion = scanner.nextInt();
            movimientos = scanner.nextInt();
            pistasUsadas = scanner.nextInt();
            tiempoInicio = scanner.nextLong();
            
            // Cargar tablero
            for (int i = 0; i < TAM; i++) {
                for (int j = 0; j < TAM; j++) {
                    tablero[i][j] = scanner.nextInt();
                }
            }
            
            // Cargar solucion
            for (int i = 0; i < TAM; i++) {
                for (int j = 0; j < TAM; j++) {
                    solucion[i][j] = scanner.nextInt();
                }
            }
            
            // Cargar celdas fijas
            for (int i = 0; i < TAM; i++) {
                for (int j = 0; j < TAM; j++) {
                    fijo[i][j] = scanner.nextInt() == 1;
                }
            }
            
            return true;
        } catch (FileNotFoundException e) {
            return false;
        }
    }
}