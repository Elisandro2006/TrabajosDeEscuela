
import os
import string

# Constantes del tablero y barcos
BOARD_SIZE = 6 
SHIP_TYPES = [
    ("Crucero", 3),
    ("Destructor", 2)
]


def clear_screen():
    os.system('cls' if os.name == 'nt' else 'clear')


def create_board(size):
    return [["~" for _ in range(size)] for _ in range(size)]


def print_board(board, show_ships=False):
    letters = string.ascii_uppercase[:len(board)]
    print("   " + " ".join(letters))
    for i, row in enumerate(board):
        line = []
        for cell in row:
            if cell == 'S' and not show_ships:
                line.append('~')
            else:
                line.append(cell)
        print(f"{i+1:2} " + " ".join(line))


def parse_coord(text):
    text = text.strip().upper()
    if len(text) < 2:
        return None
    col_letter = text[0]
    row_part = text[1:]
    if col_letter not in string.ascii_uppercase[:BOARD_SIZE]:
        return None
    try:
        row = int(row_part) - 1
    except ValueError:
        return None
    col = ord(col_letter) - ord('A')
    if 0 <= row < BOARD_SIZE and 0 <= col < BOARD_SIZE:
        return (row, col)
    return None


def can_place(board, row, col, length, orientation):
    if orientation == 'H':
        if col + length > BOARD_SIZE:
            return False
        for c in range(col, col + length):
            if board[row][c] != '~':
                return False
    else:  # V
        if row + length > BOARD_SIZE:
            return False
        for r in range(row, row + length):
            if board[r][col] != '~':
                return False
    return True


def place_ship(board, row, col, length, orientation):
    if orientation == 'H':
        for c in range(col, col + length):
            board[row][c] = 'S'
    else:
        for r in range(row, row + length):
            board[r][col] = 'S'



def place_ships_interactive(player_name, board):
    clear_screen()
    print(f"{player_name}, coloca tus barcos.")
    input("Enter para empezar...")

    for ship_name, ship_len in SHIP_TYPES:
        while True:
            clear_screen()
            print(f"{player_name} - coloca: {ship_name} (tamaño {ship_len})")
            print_board(board, show_ships=True)
            coord = input("Coordenada inicial (ej: A2): ")
            parsed = parse_coord(coord)
            if not parsed:
                print("Coordenada inválida.")
                input("Enter...")
                continue
            orientation = input("Orientación (H/V): ").upper()
            if orientation not in ('H', 'V'):
                print("Orientación inválida.")
                input("Enter...")
                continue
            r, c = parsed
            if not can_place(board, r, c, ship_len, orientation):
                print("No cabe o se cruza.")
                input("Enter...")
                continue
            place_ship(board, r, c, ship_len, orientation)
            break
    input("Listo. Enter para continuar...")

def all_ships_sunk(board):
    for row in board:
        if 'S' in row:
            return False
    return True


def take_turn(player_name, own_board, tracking_board, enemy_board):
    while True:
        clear_screen()
        print(f"Turno de {player_name}")
        print("Tu tablero:")
        print_board(own_board, show_ships=True)
        print("\nTablero enemigo (tus disparos):")
        print_board(tracking_board)
        coord = input("Disparo (ej: B3): ")
        parsed = parse_coord(coord)
        if not parsed:
            print("Inválido.")
            input("Enter...")
            continue
        r, c = parsed
        if tracking_board[r][c] in ('X', 'O'):
            print("Ya disparaste aquí.")
            input("Enter...")
            continue
        if enemy_board[r][c] == 'S':
            print("¡Tocado!")
            enemy_board[r][c] = 'X'
            tracking_board[r][c] = 'X'
            if all_ships_sunk(enemy_board):
                return True
            input("Enter...")
            return False
        else:
            print("Agua.")
            enemy_board[r][c] = 'O'
            tracking_board[r][c] = 'O'
            input("Enter...")
            return False



def main():
    board_p1 = create_board(BOARD_SIZE)
    board_p2 = create_board(BOARD_SIZE)
    tracking_p1 = create_board(BOARD_SIZE)
    tracking_p2 = create_board(BOARD_SIZE)

    place_ships_interactive("Jugador 1", board_p1)
    clear_screen()
    print("Pasa el control al Jugador 2.")
    input("Enter...")
    place_ships_interactive("Jugador 2", board_p2)

    turno = 1
    while True:
        if turno == 1:
            if take_turn("Jugador 1", board_p1, tracking_p1, board_p2):
                clear_screen()
                print("Jugador 1 ganó!")
                break
            turno = 2
        else:
            if take_turn("Jugador 2", board_p2, tracking_p2, board_p1):
                clear_screen()
                print("Jugador 2 ganó!")
                break
            turno = 1

    print("Fin del juego.")


if __name__ == '__main__':
    main()
