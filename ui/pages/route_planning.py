import customtkinter as ctk
from theme import COLORS

BG_DARK    = COLORS["bg_dark"]
BG_CARD    = COLORS["bg_card"]
BG_ROW_ALT = COLORS["bg_row_alt"]
ACCENT     = COLORS["accent"]
TEXT_MAIN  = COLORS["text_main"]
TEXT_MUTED = COLORS["text_muted"]
GREEN      = COLORS["green"]
YELLOW     = COLORS["yellow"]
RED        = COLORS["red"]
BORDER     = COLORS["border"]

DEFAULT_JUNCTIONS = [
    "Junction A",
    "Junction B",
    "Junction C",
    "Junction D",
    "Junction E",
    "Junction F",
    "Junction G"
]

class RoutePlanningPage(ctk.CTkFrame):

    def __init__(self, master, app):
        super().__init__(master, fg_color="transparent")
        self.app = app
        self.junctions = list(DEFAULT_JUNCTIONS)
        self.connections = []

        self.grid_columnconfigure(0, weight=1)
        self.grid_rowconfigure(1, weight=1)

        banner = ctk.CTkFrame(self, fg_color="#0D2137", corner_radius=8, border_width=1, border_color="#1F4B7A")
        banner.grid(row=0, column=0, sticky="ew", padx=16, pady=(12, 12))
        banner.grid_columnconfigure(0, weight=1)

        banner_text = (
            "🗺️  Route Planning Engine — 7-Junction Road Network (A–G) with Bidirectional Travel Costs "
            "& C++ Dijkstra Shortest Path Calculation"
        )
        ctk.CTkLabel(
            banner,
            text=banner_text,
            text_color="#58A6FF",
            font=ctk.CTkFont(size=12, weight="bold")
        ).pack(anchor="w", padx=16, pady=10)

        content = ctk.CTkFrame(self, fg_color="transparent")
        content.grid(row=1, column=0, sticky="nsew", padx=16, pady=(0, 16))
        content.grid_columnconfigure(0, weight=1)
        content.grid_columnconfigure(1, weight=1)
        content.grid_rowconfigure(0, weight=1)

        left_scroll = ctk.CTkScrollableFrame(
            content,
            fg_color="transparent",
            scrollbar_button_color=BORDER,
            scrollbar_button_hover_color=ACCENT
        )
        left_scroll.grid(row=0, column=0, sticky="nsew", padx=(0, 8))
        left_scroll.grid_columnconfigure(0, weight=1)
        self._build_input_panel(left_scroll)
        self._build_connections_panel(left_scroll)

        right_scroll = ctk.CTkScrollableFrame(
            content,
            fg_color="transparent",
            scrollbar_button_color=BORDER,
            scrollbar_button_hover_color=ACCENT
        )
        right_scroll.grid(row=0, column=1, sticky="nsew", padx=(8, 0))
        right_scroll.grid_columnconfigure(0, weight=1)
        self._build_result_panel(right_scroll)
        self._build_topology_panel(right_scroll)

        self.after(200, self.refresh_data)

    def _build_input_panel(self, parent):
        card = ctk.CTkFrame(parent, fg_color=BG_CARD, corner_radius=12, border_width=1, border_color=BORDER)
        card.pack(fill="x", expand=False, pady=(0, 16))

        ctk.CTkLabel(
            card,
            text="🗺️  Route Planning & Search",
            font=ctk.CTkFont(size=16, weight="bold"),
            text_color=TEXT_MAIN
        ).pack(anchor="w", padx=20, pady=(20, 4))

        ctk.CTkLabel(
            card,
            text="Select starting and destination junctions to find the lowest travel-cost route.",
            font=ctk.CTkFont(size=12),
            text_color=TEXT_MUTED
        ).pack(anchor="w", padx=20, pady=(0, 16))

        ctk.CTkFrame(card, height=1, fg_color=BORDER).pack(fill="x", padx=20, pady=(0, 16))

        grid_frame = ctk.CTkFrame(card, fg_color="transparent")
        grid_frame.pack(fill="x", padx=20, pady=(0, 12))
        grid_frame.grid_columnconfigure(0, weight=1)
        grid_frame.grid_columnconfigure(1, weight=1)

        ctk.CTkLabel(
            grid_frame,
            text="Starting Junction:",
            text_color=TEXT_MUTED,
            font=ctk.CTkFont(size=12, weight="bold")
        ).grid(row=0, column=0, sticky="w", pady=(0, 6))

        self.start_var = ctk.StringVar(value="Junction A")
        self.start_menu = ctk.CTkOptionMenu(
            grid_frame,
            variable=self.start_var,
            values=self.junctions,
            fg_color=BG_DARK,
            button_color=ACCENT,
            dropdown_fg_color=BG_CARD,
            dropdown_hover_color="#1F6FEB",
            font=ctk.CTkFont(size=13),
            corner_radius=8,
            command=self._on_junction_changed
        )
        self.start_menu.grid(row=1, column=0, sticky="ew", padx=(0, 8), pady=(0, 8))

        ctk.CTkLabel(
            grid_frame,
            text="Destination Junction:",
            text_color=TEXT_MUTED,
            font=ctk.CTkFont(size=12, weight="bold")
        ).grid(row=0, column=1, sticky="w", pady=(0, 6))

        self.end_var = ctk.StringVar(value="Junction G")
        self.end_menu = ctk.CTkOptionMenu(
            grid_frame,
            variable=self.end_var,
            values=self.junctions,
            fg_color=BG_DARK,
            button_color=ACCENT,
            dropdown_fg_color=BG_CARD,
            dropdown_hover_color="#1F6FEB",
            font=ctk.CTkFont(size=13),
            corner_radius=8,
            command=self._on_junction_changed
        )
        self.end_menu.grid(row=1, column=1, sticky="ew", padx=(8, 0), pady=(0, 8))

        self.validation_frame = ctk.CTkFrame(card, fg_color="#332200", corner_radius=6, border_width=1, border_color=YELLOW)
        self.validation_label = ctk.CTkLabel(
            self.validation_frame,
            text="⚠️  Start and destination cannot be the same junction for normal route search.",
            text_color=YELLOW,
            font=ctk.CTkFont(size=11, weight="bold")
        )
        self.validation_label.pack(padx=10, pady=6)

        self.find_btn = ctk.CTkButton(
            card,
            text="🔍  Find Shortest Route",
            fg_color=ACCENT,
            hover_color="#1557C0",
            font=ctk.CTkFont(size=14, weight="bold"),
            height=42,
            corner_radius=8,
            command=self._on_find
        )
        self.find_btn.pack(fill="x", padx=20, pady=(10, 16))

        presets_label = ctk.CTkLabel(
            card,
            text="Quick Test Route Presets:",
            font=ctk.CTkFont(size=11, weight="bold"),
            text_color=TEXT_MUTED
        )
        presets_label.pack(anchor="w", padx=20, pady=(0, 6))

        presets_frame = ctk.CTkFrame(card, fg_color="transparent")
        presets_frame.pack(fill="x", padx=20, pady=(0, 20))
        for i in range(5):
            presets_frame.grid_columnconfigure(i, weight=1)

        presets = [
            ("A ➔ G", "Junction A", "Junction G"),
            ("G ➔ A", "Junction G", "Junction A"),
            ("B ➔ F", "Junction B", "Junction F"),
            ("C ➔ E", "Junction C", "Junction E"),
            ("D ➔ G", "Junction D", "Junction G"),
        ]

        for i, (label, s, d) in enumerate(presets):
            btn = ctk.CTkButton(
                presets_frame,
                text=label,
                font=ctk.CTkFont(size=11, weight="bold"),
                fg_color=BG_DARK,
                hover_color=ACCENT,
                border_width=1,
                border_color=BORDER,
                height=30,
                corner_radius=6,
                command=lambda start=s, dest=d: self._set_and_find(start, dest)
            )
            btn.grid(row=0, column=i, padx=3, sticky="ew")

        self._check_validation()

    def _build_connections_panel(self, parent):
        card = ctk.CTkFrame(parent, fg_color=BG_CARD, corner_radius=12, border_width=1, border_color=BORDER)
        card.pack(fill="x", expand=False)

        hdr = ctk.CTkFrame(card, fg_color="transparent")
        hdr.pack(fill="x", padx=20, pady=(16, 8))
        hdr.grid_columnconfigure(0, weight=1)

        ctk.CTkLabel(
            hdr,
            text="🔗  Road Network Connections & Travel Costs",
            font=ctk.CTkFont(size=14, weight="bold"),
            text_color=TEXT_MAIN
        ).grid(row=0, column=0, sticky="w")

        self.conn_count_badge = ctk.CTkLabel(
            hdr,
            text="10 Connections",
            font=ctk.CTkFont(size=11, weight="bold"),
            text_color=ACCENT,
            fg_color="#0D2137",
            corner_radius=6,
            padx=8,
            pady=2
        )
        self.conn_count_badge.grid(row=0, column=1, sticky="e")

        ctk.CTkLabel(
            card,
            text="Positive bidirectional road edge weights defined in the C++ Graph adjacency list:",
            font=ctk.CTkFont(size=11),
            text_color=TEXT_MUTED
        ).pack(anchor="w", padx=20, pady=(0, 10))

        self.connections_container = ctk.CTkFrame(card, fg_color="transparent")
        self.connections_container.pack(fill="x", padx=16, pady=(0, 16))

        self._render_connections_list()

    def _render_connections_list(self):
        for child in self.connections_container.winfo_children():
            child.destroy()

        conns = self.connections
        if not conns:
            conns = [
                {"from": "Junction A", "to": "Junction B", "weight": 4},
                {"from": "Junction A", "to": "Junction C", "weight": 2},
                {"from": "Junction B", "to": "Junction C", "weight": 1},
                {"from": "Junction B", "to": "Junction D", "weight": 5},
                {"from": "Junction B", "to": "Junction E", "weight": 3},
                {"from": "Junction C", "to": "Junction D", "weight": 8},
                {"from": "Junction D", "to": "Junction F", "weight": 4},
                {"from": "Junction E", "to": "Junction F", "weight": 2},
                {"from": "Junction E", "to": "Junction G", "weight": 3},
                {"from": "Junction F", "to": "Junction G", "weight": 4},
            ]

        self.conn_count_badge.configure(text=f"{len(conns)} Connections")

        for idx, c in enumerate(conns):
            bg = BG_DARK if idx % 2 == 0 else BG_ROW_ALT
            row = ctk.CTkFrame(self.connections_container, fg_color=bg, corner_radius=6, height=32)
            row.pack(fill="x", pady=2)
            row.grid_columnconfigure(0, weight=1)

            conn_text = f"  {c['from']}  ◄────►  {c['to']}"
            ctk.CTkLabel(
                row,
                text=conn_text,
                font=ctk.CTkFont(family="Courier", size=12, weight="bold"),
                text_color=TEXT_MAIN
            ).grid(row=0, column=0, sticky="w", padx=8, pady=4)

            weight_text = f"Cost: {c['weight']}"
            ctk.CTkLabel(
                row,
                text=weight_text,
                font=ctk.CTkFont(size=11, weight="bold"),
                text_color=GREEN if c['weight'] <= 2 else (YELLOW if c['weight'] <= 4 else "#FF7B72"),
                fg_color="#161B22",
                corner_radius=4,
                padx=8,
                pady=2
            ).grid(row=0, column=1, sticky="e", padx=8, pady=4)

    def _build_result_panel(self, parent):
        self.result_card = ctk.CTkFrame(parent, fg_color=BG_CARD, corner_radius=12, border_width=1, border_color=BORDER)
        self.result_card.pack(fill="x", expand=False, pady=(0, 16))

        ctk.CTkLabel(
            self.result_card,
            text="📍  Dijkstra Shortest Route Result",
            font=ctk.CTkFont(size=16, weight="bold"),
            text_color=TEXT_MAIN
        ).pack(anchor="w", padx=20, pady=(20, 4))

        ctk.CTkLabel(
            self.result_card,
            text="Exact travel path and total cost calculated by C++ Graph engine.",
            font=ctk.CTkFont(size=12),
            text_color=TEXT_MUTED
        ).pack(anchor="w", padx=20, pady=(0, 16))

        ctk.CTkFrame(self.result_card, height=1, fg_color=BORDER).pack(fill="x", padx=20, pady=(0, 16))

        self.result_content = ctk.CTkFrame(self.result_card, fg_color=BG_DARK, corner_radius=8, border_width=1, border_color=BORDER)
        self.result_content.pack(fill="x", padx=20, pady=(0, 20))

        self._display_initial_state()

    def _display_initial_state(self):
        for child in self.result_content.winfo_children():
            child.destroy()

        box = ctk.CTkFrame(self.result_content, fg_color="transparent")
        box.pack(padx=20, pady=30, fill="both")

        ctk.CTkLabel(
            box,
            text="Select starting and destination junctions, then click 'Find Shortest Route'.",
            text_color=TEXT_MUTED,
            font=ctk.CTkFont(size=13),
            justify="center"
        ).pack(pady=(0, 8))

        ctk.CTkLabel(
            box,
            text="Try preset routes: A ➔ G, G ➔ A, B ➔ F, C ➔ E, D ➔ G",
            text_color=ACCENT,
            font=ctk.CTkFont(size=11)
        ).pack()

    def _display_same_junction_warning(self, junction_name):
        for child in self.result_content.winfo_children():
            child.destroy()

        box = ctk.CTkFrame(self.result_content, fg_color="transparent")
        box.pack(padx=20, pady=24, fill="both")

        badge = ctk.CTkLabel(
            box,
            text="⚠️  IDENTICAL START & DESTINATION",
            font=ctk.CTkFont(size=11, weight="bold"),
            text_color=YELLOW,
            fg_color="#332200",
            corner_radius=4,
            padx=10,
            pady=4
        )
        badge.pack(anchor="w", pady=(0, 10))

        msg = (
            f"Starting junction and destination junction are both set to \"{junction_name}\".\n\n"
            "Normal route planning requires two distinct junctions to calculate travel path.\n"
            "Please choose a different destination junction from the dropdown."
        )
        ctk.CTkLabel(
            box,
            text=msg,
            text_color=TEXT_MAIN,
            font=ctk.CTkFont(size=12),
            justify="left",
            wraplength=400
        ).pack(anchor="w")

    def _display_route_success(self, data):
        for child in self.result_content.winfo_children():
            child.destroy()

        path = data.get("path", [])
        total_cost = data.get("total_cost", 0)
        path_string = data.get("path_string", " -> ".join(path))

        box = ctk.CTkFrame(self.result_content, fg_color="transparent")
        box.pack(padx=16, pady=16, fill="both")

        status_row = ctk.CTkFrame(box, fg_color="transparent")
        status_row.pack(fill="x", pady=(0, 12))
        status_row.grid_columnconfigure(0, weight=1)

        badge = ctk.CTkLabel(
            status_row,
            text="● OPTIMAL SHORTEST ROUTE FOUND",
            font=ctk.CTkFont(size=11, weight="bold"),
            text_color=GREEN,
            fg_color="#0A2A12",
            corner_radius=4,
            padx=10,
            pady=4
        )
        badge.grid(row=0, column=0, sticky="w")

        stats_frame = ctk.CTkFrame(box, fg_color="transparent")
        stats_frame.pack(fill="x", pady=(0, 14))
        stats_frame.grid_columnconfigure(0, weight=1)
        stats_frame.grid_columnconfigure(1, weight=1)
        stats_frame.grid_columnconfigure(2, weight=1)

        c1 = ctk.CTkFrame(stats_frame, fg_color=BG_CARD, corner_radius=8, border_width=1, border_color=BORDER)
        c1.grid(row=0, column=0, padx=(0, 4), sticky="nsew")
        ctk.CTkLabel(c1, text="TOTAL COST", font=ctk.CTkFont(size=10, weight="bold"), text_color=TEXT_MUTED).pack(pady=(8, 2))
        ctk.CTkLabel(c1, text=str(total_cost), font=ctk.CTkFont(size=24, weight="bold"), text_color="#58A6FF").pack(pady=(0, 8))

        hops = max(0, len(path) - 1)
        c2 = ctk.CTkFrame(stats_frame, fg_color=BG_CARD, corner_radius=8, border_width=1, border_color=BORDER)
        c2.grid(row=0, column=1, padx=4, sticky="nsew")
        ctk.CTkLabel(c2, text="ROAD HOPS", font=ctk.CTkFont(size=10, weight="bold"), text_color=TEXT_MUTED).pack(pady=(8, 2))
        ctk.CTkLabel(c2, text=f"{hops} segs", font=ctk.CTkFont(size=20, weight="bold"), text_color=TEXT_MAIN).pack(pady=(0, 8))

        c3 = ctk.CTkFrame(stats_frame, fg_color=BG_CARD, corner_radius=8, border_width=1, border_color=BORDER)
        c3.grid(row=0, column=2, padx=(4, 0), sticky="nsew")
        ctk.CTkLabel(c3, text="JUNCTIONS", font=ctk.CTkFont(size=10, weight="bold"), text_color=TEXT_MUTED).pack(pady=(8, 2))
        ctk.CTkLabel(c3, text=str(len(path)), font=ctk.CTkFont(size=20, weight="bold"), text_color=TEXT_MAIN).pack(pady=(0, 8))

        ctk.CTkLabel(
            box,
            text="Path Sequence:",
            font=ctk.CTkFont(size=11, weight="bold"),
            text_color=TEXT_MUTED
        ).pack(anchor="w", pady=(0, 4))

        path_box = ctk.CTkFrame(box, fg_color="#101826", corner_radius=6, border_width=1, border_color="#1F4B7A")
        path_box.pack(fill="x", pady=(0, 14))

        arrow_str = "  ➜  ".join(path)
        ctk.CTkLabel(
            path_box,
            text=arrow_str,
            font=ctk.CTkFont(size=13, weight="bold"),
            text_color="#79C0FF",
            wraplength=380,
            justify="center"
        ).pack(padx=12, pady=12)

        if len(path) > 1:
            ctk.CTkLabel(
                box,
                text="Route Traversal Breakdown:",
                font=ctk.CTkFont(size=11, weight="bold"),
                text_color=TEXT_MUTED
            ).pack(anchor="w", pady=(0, 6))

            legs_frame = ctk.CTkFrame(box, fg_color="transparent")
            legs_frame.pack(fill="x")

            for i in range(len(path) - 1):
                from_j = path[i]
                to_j = path[i + 1]

                edge_cost = "?"
                for c in self.connections:
                    if (c["from"] == from_j and c["to"] == to_j) or (c["from"] == to_j and c["to"] == from_j):
                        edge_cost = str(c["weight"])
                        break

                l_row = ctk.CTkFrame(legs_frame, fg_color=BG_CARD if i % 2 == 0 else BG_ROW_ALT, corner_radius=4)
                l_row.pack(fill="x", pady=2)
                l_row.grid_columnconfigure(1, weight=1)

                ctk.CTkLabel(
                    l_row,
                    text=f"Leg {i + 1}",
                    font=ctk.CTkFont(size=11, weight="bold"),
                    text_color=TEXT_MUTED,
                    width=45
                ).grid(row=0, column=0, padx=6, pady=4, sticky="w")

                ctk.CTkLabel(
                    l_row,
                    text=f"{from_j}  ➔  {to_j}",
                    font=ctk.CTkFont(size=12),
                    text_color=TEXT_MAIN
                ).grid(row=0, column=1, padx=6, pady=4, sticky="w")

                ctk.CTkLabel(
                    l_row,
                    text=f"+{edge_cost} cost",
                    font=ctk.CTkFont(size=11, weight="bold"),
                    text_color=GREEN
                ).grid(row=0, column=2, padx=10, pady=4, sticky="e")

    def _display_error(self, message, start="", dest=""):
        for child in self.result_content.winfo_children():
            child.destroy()

        box = ctk.CTkFrame(self.result_content, fg_color="transparent")
        box.pack(padx=20, pady=24, fill="both")

        badge = ctk.CTkLabel(
            box,
            text="❌  NO ROUTE FOUND / ERROR",
            font=ctk.CTkFont(size=11, weight="bold"),
            text_color=RED,
            fg_color="#301010",
            corner_radius=4,
            padx=10,
            pady=4
        )
        badge.pack(anchor="w", pady=(0, 10))

        if start and dest:
            sub = f"Attempted path from {start} to {dest}:"
            ctk.CTkLabel(
                box,
                text=sub,
                text_color=TEXT_MUTED,
                font=ctk.CTkFont(size=12)
            ).pack(anchor="w", pady=(0, 6))

        ctk.CTkLabel(
            box,
            text=message,
            text_color=TEXT_MAIN,
            font=ctk.CTkFont(size=13, weight="bold"),
            justify="left",
            wraplength=400
        ).pack(anchor="w")

    def _build_topology_panel(self, parent):
        card = ctk.CTkFrame(parent, fg_color=BG_CARD, corner_radius=12, border_width=1, border_color=BORDER)
        card.pack(fill="x", expand=False)

        ctk.CTkLabel(
            card,
            text="🗺️  Network Topology Diagram",
            font=ctk.CTkFont(size=14, weight="bold"),
            text_color=TEXT_MAIN
        ).pack(anchor="w", padx=20, pady=(16, 4))

        ctk.CTkLabel(
            card,
            text="Visual representation of the 7 junctions and positive road weights:",
            font=ctk.CTkFont(size=11),
            text_color=TEXT_MUTED
        ).pack(anchor="w", padx=20, pady=(0, 10))

        diagram_text = (
            "          (4)\n"
            "      [A] ──── [B] ──(5)── [D]\n"
            "       │        │ \\         │\n"
            "      (2)      (1) (3)     (4)\n"
            "       │        │   \\       │\n"
            "      [C] ──────┘    [E] ── [F]\n"
            "                      │ \\   │\n"
            "                     (3) \\ (4)\n"
            "                      │   \\ │\n"
            "                     [G] ───┘\n"
        )

        diag_frame = ctk.CTkFrame(card, fg_color=BG_DARK, corner_radius=8, border_width=1, border_color=BORDER)
        diag_frame.pack(fill="x", padx=16, pady=(0, 12))

        ctk.CTkLabel(
            diag_frame,
            text=diagram_text,
            font=ctk.CTkFont(family="Courier", size=11, weight="bold"),
            text_color="#58A6FF",
            justify="left"
        ).pack(padx=16, pady=12, anchor="center")

        stats_note = (
            "• Adjacency List: Undirected graph with 7 vertices (A–G) and 10 edges.\n"
            "• Bidirectional travel: Edge weights apply equally in both directions.\n"
            "• Dynamic Updates: Road weights automatically adapt based on traffic levels."
        )
        ctk.CTkLabel(
            card,
            text=stats_note,
            font=ctk.CTkFont(size=11),
            text_color=TEXT_MUTED,
            justify="left"
        ).pack(anchor="w", padx=20, pady=(0, 16))

    def _on_junction_changed(self, _=None):
        self._check_validation()

    def _check_validation(self):
        start = self.start_var.get()
        dest  = self.end_var.get()

        if start == dest:
            self.validation_frame.pack(fill="x", padx=20, pady=(0, 8), before=self.find_btn)
            self.find_btn.configure(fg_color="#444C56", hover_color="#444C56")
        else:
            self.validation_frame.pack_forget()
            self.find_btn.configure(fg_color=ACCENT, hover_color="#1557C0")

    def _set_and_find(self, start, dest):
        self.start_var.set(start)
        self.end_var.set(dest)
        self._check_validation()
        self._on_find()

    def _on_find(self):
        start = self.start_var.get()
        dest  = self.end_var.get()

        if start == dest:
            self._display_same_junction_warning(start)
            if hasattr(self.app, "log_activity"):
                self.app.log_activity(
                    f"Route search rejected: start and destination are both \"{start}\".",
                    False
                )
            return

        if not self.app.backend or not self.app.backend.connected:
            self._display_error("Backend engine is not connected.", start, dest)
            return

        req = {
            "operation": "find_shortest_path",
            "start": start,
            "destination": dest
        }

        res = self.app.backend.send_request(req)

        if res.get("success") and res.get("data"):
            data = res["data"]
            self._display_route_success(data)
            if hasattr(self.app, "log_activity"):
                self.app.log_activity(
                    f"Calculated shortest route ({start} ➔ {dest}): {data.get('path_string')} | Cost: {data.get('total_cost')}",
                    True
                )
        else:
            err_msg = res.get("message", "No available route found.")
            self._display_error(err_msg, start, dest)
            if hasattr(self.app, "log_activity"):
                self.app.log_activity(
                    f"Route calculation failed ({start} ➔ {dest}): {err_msg}",
                    False
                )

    def refresh_data(self):
        if not self.app.backend or not self.app.backend.connected:
            return

        res_j = self.app.backend.send_request({"operation": "get_junctions"})
        if res_j.get("success") and "data" in res_j and "junctions" in res_j["data"]:
            names = [j["name"] for j in res_j["data"]["junctions"]]
            if names:
                self.junctions = names
                self.start_menu.configure(values=names)
                self.end_menu.configure(values=names)

                if self.start_var.get() not in names:
                    self.start_var.set(names[0])
                if self.end_var.get() not in names:
                    self.end_var.set(names[-1] if len(names) > 1 else names[0])

        res_c = self.app.backend.send_request({"operation": "get_graph_connections"})
        if res_c.get("success") and "data" in res_c and "connections" in res_c["data"]:
            self.connections = res_c["data"]["connections"]
            self._render_connections_list()

        self._check_validation()
