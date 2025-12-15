using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading;

class SnakeJuego
{
    static int ancho = 30;
    static int alto = 15;
    static List<(int x, int y)> snake = new List<(int x, int y)>();
    static (int x, int y) direccion = (1, 0);
    static (int x, int y) comida;
    static (int x, int y) trampa;
    static int nivel = 1;
    static int puntaje = 0;
    static Random random = new Random();
    static bool juegoTerminado = false;
    static string nombreJugador = "";

    static string archivoPartidas = "partidas.txt";

    static void Main()
    {
        Console.Title = "🐍 Snake Niveles Infinitos 🐍";
        Console.CursorVisible = false;
        MenuPrincipal();
    }

    // ==============================
    // MENÚ PRINCIPAL
    // ==============================
    static void MenuPrincipal()
    {
        while (true)
        {
            Console.Clear();
            Console.WriteLine("=== 🐍 SNAKE: NIVELES INFINITOS 🐍 ===");
            Console.WriteLine("\n1. Nueva partida");
            Console.WriteLine("2. Cargar partida guardada");
            Console.WriteLine("3. Ver ranking");
            Console.WriteLine("4. Salir");
            Console.Write("\nElige una opción: ");
            string opcion = Console.ReadLine();

            switch (opcion)
            {
                case "1":
                    NuevaPartida();
                    break;
                case "2":
                    CargarPartida();
                    break;
                case "3":
                    MostrarRanking();
                    break;
                case "4":
                    Console.WriteLine("\n¡Hasta pronto!");
                    return;
                default:
                    Console.WriteLine("Opción inválida. Presiona una tecla para continuar...");
                    Console.ReadKey();
                    break;
            }
        }
    }

    // ==============================
    // NUEVA PARTIDA
    // ==============================
    static void NuevaPartida()
    {
        Console.Clear();
        Console.Write("Ingresa tu nombre: ");
        nombreJugador = Console.ReadLine();

        nivel = 1;
        puntaje = 0;
        IniciarJuego();
        Jugar();
    }

    // ==============================
    // CARGAR PARTIDA
    // ==============================
    static void CargarPartida()
    {
        if (!File.Exists(archivoPartidas))
        {
            Console.WriteLine("No hay partidas guardadas.");
            Console.ReadKey();
            return;
        }

        string[] lineas = File.ReadAllLines(archivoPartidas);
        if (lineas.Length == 0)
        {
            Console.WriteLine("No hay partidas guardadas.");
            Console.ReadKey();
            return;
        }

        Console.Clear();
        Console.WriteLine("=== PARTIDAS GUARDADAS ===");
        for (int i = 0; i < lineas.Length; i++)
        {
            string[] datos = lineas[i].Split(',');
            Console.WriteLine($"{i + 1}. {datos[0]} - Nivel {datos[1]} - Puntaje {datos[2]}");
        }

        Console.Write("\nElige el número de partida a cargar: ");
        if (int.TryParse(Console.ReadLine(), out int eleccion) && eleccion > 0 && eleccion <= lineas.Length)
        {
            string[] datos = lineas[eleccion - 1].Split(',');
            nombreJugador = datos[0];
            nivel = int.Parse(datos[1]);
            puntaje = int.Parse(datos[2]);
            Console.WriteLine($"\nPartida de {nombreJugador} cargada. Nivel {nivel}, Puntaje {puntaje}");
            Console.ReadKey();
            IniciarJuego();
            Jugar();
        }
        else
        {
            Console.WriteLine("Selección inválida.");
            Console.ReadKey();
        }
    }

    // ==============================
    // JUEGO PRINCIPAL
    // ==============================
    static void Jugar()
    {
        juegoTerminado = false;

        while (!juegoTerminado)
        {
            DibujarBordes();
            MostrarInfo();
            LeerMovimiento();
            MoverSnake();
            Thread.Sleep(Math.Max(50, 200 - nivel * 10));
        }

        Console.Clear();
        Console.WriteLine($"Juego terminado. Puntaje final: {puntaje}");
        GuardarPartida();
        Console.WriteLine("Partida guardada. Presiona una tecla para volver al menú...");
        Console.ReadKey();
    }

    // ==============================
    // INICIALIZAR
    // ==============================
    static void IniciarJuego()
    {
        snake.Clear();
        int inicioX = ancho / 2;
        int inicioY = alto / 2;

        for (int i = 0; i < 5; i++)
            snake.Add((inicioX - i, inicioY));

        direccion = (1, 0);
        NuevaComida();
        NuevaTrampa();
    }

    // ==============================
    // LÓGICA DEL JUEGO
    // ==============================
    static void DibujarBordes()
    {
        Console.SetCursorPosition(0, 0);
        for (int y = 0; y <= alto; y++)
        {
            for (int x = 0; x <= ancho; x++)
            {
                if (x == 0 || y == 0 || x == ancho || y == alto)
                    Console.Write("#");
                else if (snake[0].x == x && snake[0].y == y)
                    Console.Write("O");
                else if (snake.Contains((x, y)))
                    Console.Write("o");
                else if (comida.x == x && comida.y == y)
                    Console.Write("+");
                else if (trampa.x == x && trampa.y == y)
                    Console.Write("X");
                else
                    Console.Write(" ");
            }
            Console.WriteLine();
        }
    }

    static void MostrarInfo()
    {
        Console.SetCursorPosition(0, alto + 2);
        Console.Write($"Jugador: {nombreJugador} | Nivel: {nivel} | Puntaje: {puntaje} | Tamaño: {snake.Count}   ");
    }

    static void LeerMovimiento()
    {
        if (Console.KeyAvailable)
        {
            var tecla = Console.ReadKey(true).Key;
            if (tecla == ConsoleKey.UpArrow && direccion.y != 1) direccion = (0, -1);
            else if (tecla == ConsoleKey.DownArrow && direccion.y != -1) direccion = (0, 1);
            else if (tecla == ConsoleKey.LeftArrow && direccion.x != 1) direccion = (-1, 0);
            else if (tecla == ConsoleKey.RightArrow && direccion.x != -1) direccion = (1, 0);
        }
    }

    static void MoverSnake()
    {
        (int x, int y) cabeza = (snake[0].x + direccion.x, snake[0].y + direccion.y);

        if (cabeza.x <= 0) cabeza.x = ancho - 1;
        else if (cabeza.x >= ancho) cabeza.x = 1;
        if (cabeza.y <= 0) cabeza.y = alto - 1;
        else if (cabeza.y >= alto) cabeza.y = 1;

        if (snake.Contains(cabeza))
        {
            juegoTerminado = true;
            return;
        }

        snake.Insert(0, cabeza);

        if (cabeza == comida)
        {
            puntaje += 10;
            nivel++;
            NuevaComida();
            NuevaTrampa();
        }
        else if (cabeza == trampa)
        {
            if (snake.Count > 1)
                snake.RemoveAt(snake.Count - 1);
            else
                juegoTerminado = true;

            puntaje = Math.Max(0, puntaje - 5);
            nivel++;
            NuevaTrampa();
            NuevaComida();
        }
        else
        {
            snake.RemoveAt(snake.Count - 1);
        }
    }

    // ==============================
    // COMIDA Y TRAMPAS
    // ==============================
    static void NuevaComida()
    {
        comida = (random.Next(1, ancho - 1), random.Next(1, alto - 1));
        while (snake.Contains(comida) || comida == trampa)
            comida = (random.Next(1, ancho - 1), random.Next(1, alto - 1));
    }

    static void NuevaTrampa()
    {
        trampa = (random.Next(1, ancho - 1), random.Next(1, alto - 1));
        while (snake.Contains(trampa) || trampa == comida)
            trampa = (random.Next(1, ancho - 1), random.Next(1, alto - 1));
    }

    // ==============================
    // GUARDADO Y RANKING
    // ==============================
    static void GuardarPartida()
    {
        List<string> partidas = new List<string>();
        if (File.Exists(archivoPartidas))
            partidas = File.ReadAllLines(archivoPartidas).ToList();

        // Reemplaza si ya existía el jugador
        bool reemplazado = false;
        for (int i = 0; i < partidas.Count; i++)
        {
            string[] datos = partidas[i].Split(',');
            if (datos[0] == nombreJugador)
            {
                partidas[i] = $"{nombreJugador},{nivel},{puntaje}";
                reemplazado = true;
                break;
            }
        }
        if (!reemplazado)
            partidas.Add($"{nombreJugador},{nivel},{puntaje}");

        File.WriteAllLines(archivoPartidas, partidas);
    }

    static void MostrarRanking()
    {
        if (!File.Exists(archivoPartidas))
        {
            Console.WriteLine("No hay partidas guardadas.");
            Console.ReadKey();
            return;
        }

        var partidas = File.ReadAllLines(archivoPartidas)
            .Select(l => l.Split(','))
            .Select(d => new { Nombre = d[0], Nivel = int.Parse(d[1]), Puntaje = int.Parse(d[2]) })
            .OrderByDescending(p => p.Puntaje)
            .ToList();

        Console.Clear();
        Console.WriteLine("=== 🏆 RANKING DE JUGADORES 🏆 ===\n");

        int pos = 1;
        foreach (var p in partidas)
        {
            Console.WriteLine($"{pos}. {p.Nombre} - Puntaje: {p.Puntaje} - Nivel: {p.Nivel}");
            pos++;
        }

        Console.WriteLine("\nPresiona una tecla para volver al menú...");
        Console.ReadKey();
    }
}
