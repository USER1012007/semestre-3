# nix-shell -p python313Packages.tkinter
# librerias
import tkinter as tk
from tkinter import ttk, messagebox

# constantes fijas
PEGS = ['A', 'B', 'C']
COLORS = ['#e74c3c', '#e67e22', '#f1c40f', '#2ecc71', '#3498db', '#9b59b6', '#1abc9c']


# objeto para inicializar la gui
class Hanoi:
    # funcion que se inicializa al instanciar un objeto de la clase Hanoi
    def __init__(self, root):
        self.root = root
        root.title("Torres de Hanoi")

        # inicia la gui
        top = tk.Frame(root)
        top.pack(pady=5)
        tk.Label(top, text="numero de discos (1-7):").pack(side=tk.LEFT)
        self.n_var = tk.StringVar(value="3")
        tk.Entry(top, textvariable=self.n_var, width=4).pack(side=tk.LEFT)

        # genera los botones principales de la gui
        tk.Button(top, text="Start", command=self.start).pack(side=tk.LEFT, padx=2)
        tk.Button(top, text="Pause", command=self.pause).pack(side=tk.LEFT, padx=2)
        tk.Button(top, text="Continue", command=self.cont).pack(side=tk.LEFT, padx=2)
        tk.Button(top, text="Quit", command=self.quit_game).pack(side=tk.LEFT, padx=2)
        tk.Button(top, text="Exit", command=root.destroy).pack(side=tk.LEFT, padx=2)
        self.stack_btn = tk.Button(top, text="Stack", command=self.show_stack, state=tk.DISABLED)
        self.stack_btn.pack(side=tk.LEFT, padx=2)

        # defino el label para la velocidad
        speed_f = tk.Frame(root)
        speed_f.pack()
        tk.Label(speed_f, text="Velocidad (ms):").pack(side=tk.LEFT)
        self.speed = tk.Scale(speed_f, from_=50, to=1500, orient=tk.HORIZONTAL, length=200)
        self.speed.set(500)
        self.speed.pack(side=tk.LEFT)


        # variables neesarias para la gui
        self.canvas = tk.Canvas(root, width=600, height=300, bg="white")
        self.canvas.pack(pady=10)

        self.moves = []
        self.events = []
        self.move_idx = 0
        self.paused = False
        self.running = False
        self.pegs = {p: [] for p in PEGS}
        self.peg_x = {'A': 130, 'B': 300, 'C': 470}

    # funcion para validar que se esten ingresando correctamente el numero de discos
    def validate_n(self):
        try:
            n = int(self.n_var.get())
        except ValueError:
            messagebox.showerror("Error", "Ingresa un numero entero.")
            return None
        if n < 1 or n > 7:
            messagebox.showerror("Error", "Numero de discos debe ser entre 1 y 7.")
            return None
        return n

    # funcion principal de la logica del programa
    def build_events(self, n):
        self.moves = []
        self.events = []
        stack = []
        counter = [0]

        # funcion recursiva de la torre de hanoi
        def hanoi(n, origen, destino, auxiliar):
            counter[0] += 1
            idx = counter[0]
            # aqui almaceno los valores para mostrarlos en la pila
            stack.append(f"hanoi({n},{origen},{destino},{auxiliar})")
            varstr = f"n={n}, origen={origen}, destino={destino}, auxiliar={auxiliar}"
            pending = (f"mover disco {n}: {origen}->{destino}" if n == 1
                       else f"hanoi({n-1},{origen},{auxiliar},{destino})")
            self.events.append((idx, "PUSH", list(reversed(stack)), varstr, pending, "-"))

            if n == 1:
                self.moves.append((n, origen, destino))
                ret = "hecho"

            else:
                hanoi(n - 1, origen, auxiliar, destino)
                self.moves.append((n, origen, destino))
                hanoi(n - 1, auxiliar, destino, origen)
                ret = "hecho"
            # aqui almaceno los valores para mostrarlos en la pila
            self.events.append((idx, "POP", list(reversed(stack)), varstr,
                                 f"retorno de hanoi({n})", ret))
            stack.pop()

        # llamada principal con los valores iniciales de la funcion recursiva
        hanoi(n, 'A', 'C', 'B')

    # funcion principal para dibujar la interfaz grafica
    def draw_initial(self, n):
        self.canvas.delete("all")
        for p in PEGS:
            x = self.peg_x[p]
            self.canvas.create_rectangle(x - 5, 60, x + 5, 260, fill="black")
            self.canvas.create_text(x, 275, text=p, font=("Arial", 14, "bold"))
        self.pegs = {p: [] for p in PEGS}
        for size in range(n, 0, -1):
            self.pegs['A'].append(size)
        self.redraw_pegs()

    # funcion para renderizar el cambio de posicion de los discos
    def redraw_pegs(self):
        self.canvas.delete("disc")
        for p in PEGS:
            for pos, size in enumerate(self.pegs[p]):
                x = self.peg_x[p]
                y = 250 - pos * 20
                w = 15 + size * 15
                color = COLORS[(size - 1) % len(COLORS)]
                self.canvas.create_rectangle(x - w / 2, y - 18, x + w / 2, y, fill=color, tags="disc")

    # funcion para inicializar todo
    def start(self):
        n = self.validate_n()
        if n is None:
            return
        self.build_events(n)
        self.draw_initial(n)
        self.move_idx = 0
        self.paused = False
        self.running = True
        self.stack_btn.config(state=tk.DISABLED)
        self.step()

    # funcion principal para inicializar el renderizado del cambio de posicion de los discos 
    def step(self):
        if not self.running or self.paused:
            return
        if self.move_idx >= len(self.moves):
            self.running = False
            self.stack_btn.config(state=tk.NORMAL)
            return
        size, origen, destino = self.moves[self.move_idx]
        self.pegs[origen].remove(size)
        self.pegs[destino].append(size)
        self.redraw_pegs()
        self.move_idx += 1
        self.root.after(self.speed.get(), self.step)

    # funcion para pausar el programa
    def pause(self):
        self.paused = True

    # funcion para continuar el programa 
    def cont(self):
        if self.paused:
            self.paused = False
            self.step()

    # funcion para salir del programa
    def quit_game(self):
        self.running = False
        self.paused = False
        self.canvas.delete("all")
        self.move_idx = 0
        self.stack_btn.config(state=tk.DISABLED)

    # funcion para mostrar la pila de operaciones
    def show_stack(self):
        win = tk.Toplevel(self.root)
        win.title("Pila (Stack)")
        cols = ("indice", "operacion", "pilas", "variable", "pendiente")
        heads = ("Indice", "Operacion", "Estado de la pila", "Variables locales",
                 "Accion pendiente")
        tree = ttk.Treeview(win, columns=cols, show="headings", height=20)
        for c, h in zip(cols, heads):
            tree.heading(c, text=h)
            tree.column(c, width=160)
        for idx, op, pila, varstr, pending, ret in self.events:
            tree.insert("", tk.END, values=(idx, op, " | ".join(pila), varstr, pending))
        tree.pack(fill=tk.BOTH, expand=True)


# funcion principal del loop
if __name__ == "__main__":
    root = tk.Tk()
    app = Hanoi(root)
    root.mainloop()
