using System;
using System.Collections.Generic;
using System.Threading;

class LaberintoJuego
{
    static int width = 21;
    static int height = 21;
    static char[,] maze;
    static int[,] visibilidad;
    static Random random = new Random();

    static int jugadorX = 1, jugadorY = 1;
    static int salidaX, salidaY;
    static bool juegoTerminado = false;

    static int vidas = 3;
    static int trampasTotales = 10;
    static int tesorosTotales = 10;
    static int pocionesTotales = 5;

    static int nivel = 1;
    static int maxNiveles = 3;

    static Dictionary<string, int> inventario = new Dictionary<string, int>()
    {
        {"Bronce",0},
        {"Plata",0},
        {"Oro",0},
        {"Pocion",0}
    };

    static void Main()
    {
        Console.OutputEncoding = System.Text.Encoding.UTF8;
        IniciarNivel();
        while (!juegoTerminado)
        {
            MostrarLaberinto();
            MovimientoJugador();
        }
    }

    static void IniciarNivel()
    {
        juegoTerminado = false;
        maze = new char[height, width];
        visibilidad = new int[height, width];

        InicializarLaberinto();
        GenerarLaberinto(1, 1);
        ColocarTrampas();
        ColocarTesoros();
        ColocarPociones();

        maze[1, 0] = ' ';
        maze[height - 2, width - 1] = ' ';
        salidaX = width - 2;
        salidaY = height - 2;

        jugadorX = 1;
        jugadorY = 1;

        // Restaurar vidas al iniciar cada nivel
        vidas = 3;

        AnimarNiebla();
    }

    static void InicializarLaberinto()
    {
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                maze[y, x] = '█';
    }

    static void GenerarLaberinto(int x, int y)
    {
        maze[y, x] = ' ';
        int[,] dirs = { { 0, -2 }, { 0, 2 }, { -2, 0 }, { 2, 0 } };
        for (int i = 0; i < 4; i++)
        {
            int j = random.Next(4);
            (dirs[i, 0], dirs[j, 0]) = (dirs[j, 0], dirs[i, 0]);
            (dirs[i, 1], dirs[j, 1]) = (dirs[j, 1], dirs[i, 1]);
        }
        for (int i = 0; i < 4; i++)
        {
            int nx = x + dirs[i, 0];
            int ny = y + dirs[i, 1];
            if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && maze[ny, nx] == '█')
            {
                maze[y + dirs[i, 1] / 2, x + dirs[i, 0] / 2] = ' ';
                GenerarLaberinto(nx, ny);
            }
        }
    }

    static void ColocarTrampas()
    {
        int colocadas = 0;
        while (colocadas < trampasTotales)
        {
            int x = random.Next(1, width - 1);
            int y = random.Next(1, height - 1);
            if (maze[y, x] == ' ' && (x != 1 || y != 1))
            {
                maze[y, x] = '^';
                colocadas++;
            }
        }
    }

    static void ColocarTesoros()
    {
        int colocados = 0;
        while (colocados < tesorosTotales)
        {
            int x = random.Next(1, width - 1);
            int y = random.Next(1, height - 1);
            if (maze[y, x] == ' ' && (x != 1 || y != 1))
            {
                int tipo = random.Next(3);
                switch (tipo)
                {
                    case 0: maze[y, x] = 'b'; break;
                    case 1: maze[y, x] = 'p'; break;
                    case 2: maze[y, x] = 'o'; break;
                }
                colocados++;
            }
        }
    }

    static void ColocarPociones()
    {
        int colocadas = 0;
        while (colocadas < pocionesTotales)
        {
            int x = random.Next(1, width - 1);
            int y = random.Next(1, height - 1);
            if (maze[y, x] == ' ' && (x != 1 || y != 1))
            {
                maze[y, x] = 'H';
                colocadas++;
            }
        }
    }

    static void MostrarLaberinto()
    {
        Console.Clear();
        Console.WriteLine($"🌀 LABERINTO DEL TESORO 🌀   ❤️ Vidas: {vidas}   🔢 Nivel: {nivel}\n");

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int vis = visibilidad[y, x];
                if (vis == 0) { Console.ForegroundColor = ConsoleColor.DarkGray; Console.Write('░'); }
                else if (vis == 1) { Console.ForegroundColor = ConsoleColor.Gray; Console.Write('▒'); }
                else if (vis == 2) { Console.ForegroundColor = ConsoleColor.Gray; Console.Write('▓'); }
                else if (vis == 3) { Console.ForegroundColor = ConsoleColor.White; Console.Write('▒'); }
                else if (vis >= 4)
                {
                    if (x == jugadorX && y == jugadorY) { Console.ForegroundColor = ConsoleColor.Yellow; Console.Write('☺'); }
                    else if (x == salidaX && y == salidaY) { Console.ForegroundColor = ConsoleColor.Red; Console.Write('⬤'); }
                    else if (maze[y, x] == '█') { Console.ForegroundColor = ConsoleColor.DarkGray; Console.Write('█'); }
                    else if (maze[y, x] == '^') { Console.ForegroundColor = ConsoleColor.Magenta; Console.Write('^'); }
                    else if (maze[y, x] == 'b') { Console.ForegroundColor = ConsoleColor.DarkYellow; Console.Write('●'); }
                    else if (maze[y, x] == 'p') { Console.ForegroundColor = ConsoleColor.White; Console.Write('●'); }
                    else if (maze[y, x] == 'o') { Console.ForegroundColor = ConsoleColor.Yellow; Console.Write('★'); }
                    else if (maze[y, x] == 'H') { Console.ForegroundColor = ConsoleColor.Cyan; Console.Write('♥'); }
                    else { Console.Write(' '); }
                }
            }
            Console.WriteLine();
        }

        Console.ForegroundColor = ConsoleColor.Cyan;
        Console.WriteLine("\nWASD/Flechas = mover, U = usar poción, I = inventario");
    }

    static void MovimientoJugador()
    {
        ConsoleKey key = Console.ReadKey(true).Key;
        int newX = jugadorX, newY = jugadorY;

        switch (key)
        {
            case ConsoleKey.W: case ConsoleKey.UpArrow: newY--; break;
            case ConsoleKey.S: case ConsoleKey.DownArrow: newY++; break;
            case ConsoleKey.A: case ConsoleKey.LeftArrow: newX--; break;
            case ConsoleKey.D: case ConsoleKey.RightArrow: newX++; break;
            case ConsoleKey.I: MostrarInventario(); return;
            case ConsoleKey.U: UsarPocion(); return;
            default: return;
        }

        if (newX >= 0 && newX < width && newY >= 0 && newY < height && maze[newY, newX] != '█')
        {
            jugadorX = newX;
            jugadorY = newY;

            if (maze[newY, newX] == '^') { vidas--; maze[newY, newX] = ' '; if (vidas <= 0) { GameOver(); return; } Console.ForegroundColor = ConsoleColor.Red; Console.WriteLine("\n💥 ¡Has caído en una trampa! Pierdes una vida 💥"); Console.ResetColor(); Thread.Sleep(700); }
            if (maze[newY, newX] == 'b') { inventario["Bronce"]++; maze[newY, newX] = ' '; MensajeTesoro("Bronce", 1); }
            if (maze[newY, newX] == 'p') { inventario["Plata"]++; maze[newY, newX] = ' '; MensajeTesoro("Plata", 3); }
            if (maze[newY, newX] == 'o') { inventario["Oro"]++; maze[newY, newX] = ' '; MensajeTesoro("Oro", 5); }
            if (maze[newY, newX] == 'H') { inventario["Pocion"]++; maze[newY, newX] = ' '; Console.ForegroundColor = ConsoleColor.Cyan; Console.WriteLine("\n💖 Has recogido una poción!"); Console.ResetColor(); Thread.Sleep(500); }

            AnimarNiebla();
        }

        if (jugadorX == salidaX && jugadorY == salidaY) Victoria();
    }

    static void AnimarNiebla()
    {
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                if (visibilidad[y, x] > 1 && visibilidad[y, x] < 5)
                    visibilidad[y, x]--;

        for (int dy = -1; dy <= 1; dy++)
            for (int dx = -1; dx <= 1; dx++)
            {
                int nx = jugadorX + dx;
                int ny = jugadorY + dy;
                if (nx >= 0 && nx < width && ny >= 0 && ny < height)
                    visibilidad[ny, nx] = 5;
            }
    }

    static void MostrarInventario()
    {
        Console.Clear();
        Console.ForegroundColor = ConsoleColor.Yellow;
        Console.WriteLine("📦 INVENTARIO DEL JUGADOR 📦\n");
        int totalPuntos = inventario["Bronce"] * 1 + inventario["Plata"] * 3 + inventario["Oro"] * 5;
        Console.ForegroundColor = ConsoleColor.DarkYellow; Console.WriteLine($"Bronce: {inventario["Bronce"]}");
        Console.ForegroundColor = ConsoleColor.White; Console.WriteLine($"Plata: {inventario["Plata"]}");
        Console.ForegroundColor = ConsoleColor.Yellow; Console.WriteLine($"Oro: {inventario["Oro"]}");
        Console.ForegroundColor = ConsoleColor.Cyan; Console.WriteLine($"Pociones: {inventario["Pocion"]}");
        Console.ForegroundColor = ConsoleColor.Green; Console.WriteLine($"\nPuntuación total: {totalPuntos}");
        Console.ForegroundColor = ConsoleColor.Cyan; Console.WriteLine("\nPresiona cualquier tecla para volver...");
        Console.ReadKey(true);
    }

    static void UsarPocion()
    {
        if (inventario["Pocion"] > 0 && vidas < 3)
        {
            inventario["Pocion"]--;
            vidas++;
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine("\n💖 Has usado una poción. +1 vida!");
            Console.ResetColor();
            Thread.Sleep(500);
        }
        else
        {
            Console.ForegroundColor = ConsoleColor.Gray;
            Console.WriteLine("\n⚠️ No puedes usar poción ahora.");
            Console.ResetColor();
            Thread.Sleep(500);
        }
    }

    static void MensajeTesoro(string tipo, int valor)
    {
        Console.ForegroundColor = ConsoleColor.Green;
        Console.WriteLine($"\n💎 Has encontrado un tesoro de {tipo}! (+{valor} pts)");
        Console.ResetColor();
        Thread.Sleep(500);
    }

    static void GameOver()
    {
        juegoTerminado = true;
        Console.Clear();
        Console.ForegroundColor = ConsoleColor.Red;
        Console.WriteLine("\n💀 GAME OVER 💀");
        MostrarInventario();
        Environment.Exit(0);
    }

    static void Victoria()
    {
        juegoTerminado = true;
        Console.Clear();
        Console.ForegroundColor = ConsoleColor.Green;
        Console.WriteLine($"\n🎉 ¡HAS ENCONTRADO LA SALIDA DEL NIVEL {nivel}! 🎉");
        MostrarInventario();

        if (nivel < maxNiveles)
        {
            Console.ForegroundColor = ConsoleColor.Cyan;
            Console.WriteLine("\n¿Deseas avanzar al siguiente nivel? (S/N)");
            ConsoleKey key = Console.ReadKey(true).Key;
            if (key == ConsoleKey.S)
            {
                nivel++;
                IniciarNivel(); // Reinicia el laberinto y restaura vidas
            }
            else
            {
                Console.WriteLine("\nContinuar explorando el nivel actual...");
                juegoTerminado = false;
                Thread.Sleep(1000);
            }
        }
        else
        {
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine("\n🎯 ¡Has completado todos los niveles! Puntaje final:");
            MostrarInventario();
            Environment.Exit(0);
        }
    }
}
