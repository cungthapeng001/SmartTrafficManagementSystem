import customtkinter as ctk
from theme import COLORS, get_traffic_level

ROAD_JUNCTIONS = {
    "NORTH": "Junction A ↔ Junction B",
    "SOUTH": "Junction C ↔ Junction D",
    "EAST":  "Junction B ↔ Junction D",
    "WEST":  "Junction A ↔ Junction C",
    "CENTRAL": "Junction A ↔ Junction D",
}

class RoadManagementPage(ctk.CTkFrame):

    def __init__(self, master, app):
        super().__init__(master, fg_color="transparent")
        self.app = app
        self.roads_data = []

        self.grid_columnconfigure((0, 1), weight=1)
        self.grid_rowconfigure(1, weight=1)

        self._build_banner()

        self._build_add_road_panel()

        self._build_roads_list_panel()

    def _build_banner(self):
        banner = ctk.CTkFrame(self, fg_color="#0D2137", corner_radius=8)
        banner.grid(row=0, column=0, columnspan=2, sticky="ew", padx=16, pady=(16, 8))

        ctk.CTkLabel(
            banner,
            text="🛣️  Multi-Road Management — Configure active roads, simulated traffic sensors, "
                 "and junction connections for graph topology.",
            font=ctk.CTkFont(size=12),
            text_color=COLORS["accent"]
        ).pack(padx=16, pady=10)

    def _build_add_road_panel(self):
        card = ctk.CTkFrame(
            self, fg_color=COLORS["bg_card"], corner_radius=12,
            border_width=1, border_color=COLORS["border"]
        )
        card.grid(row=1, column=0, padx=(16, 8), pady=8, sticky="nsew")
        card.grid_columnconfigure(0, weight=1)

        ctk.CTkLabel(
            card, text="➕  Add New Road",
            font=ctk.CTkFont(size=16, weight="bold"),
            text_color=COLORS["text_main"]
        ).grid(row=0, column=0, padx=16, pady=(16, 4), sticky="w")

        ctk.CTkLabel(
            card, text="Register an additional road in the C++ backend engine.",
            font=ctk.CTkFont(size=11), text_color=COLORS["text_muted"]
        ).grid(row=1, column=0, padx=16, pady=(0, 12), sticky="w")

        ctk.CTkFrame(card, height=1, fg_color=COLORS["border"]).grid(
            row=2, column=0, padx=16, pady=(0, 14), sticky="ew"
        )

        self.road_id_var = ctk.StringVar(value="")
        self.road_name_var = ctk.StringVar(value="")
        self.junction_var = ctk.StringVar(value="Junction A ↔ Junction B")
        self.sensor_count_var = ctk.StringVar(value="10")
        self.max_wait_var = ctk.StringVar(value="30")

        def make_field(r, label, var, placeholder):
            ctk.CTkLabel(
                card, text=label, font=ctk.CTkFont(size=12),
                text_color=COLORS["text_muted"], anchor="w"
            ).grid(row=r, column=0, padx=16, pady=(0, 2), sticky="w")
            entry = ctk.CTkEntry(
                card, textvariable=var, placeholder_text=placeholder,
                fg_color=COLORS["bg_dark"], border_color=COLORS["border"],
                text_color=COLORS["text_main"], corner_radius=8, height=36
            )
            entry.grid(row=r+1, column=0, padx=16, pady=(0, 10), sticky="ew")
            return entry

        make_field(3, "Road ID (e.g. ROAD_A, EAST_EXPR):", self.road_id_var, "e.g. ROAD_A")
        make_field(5, "Display Name (e.g. Road A):", self.road_name_var, "e.g. Road A")
        make_field(7, "Junction Connections (Graph Topology):", self.junction_var, "e.g. Junction A ↔ Junction C")
        make_field(9, "Simulated Vehicle Count:", self.sensor_count_var, "e.g. 10")
        make_field(11, "Simulated Max Wait Time (s):", self.max_wait_var, "e.g. 30")

        ctk.CTkButton(
            card, text="➕  Register Road in Backend",
            fg_color=COLORS["green"], hover_color="#237a35",
            font=ctk.CTkFont(size=13, weight="bold"),
            height=40, corner_radius=8,
            command=self.add_road
        ).grid(row=13, column=0, padx=16, pady=(8, 16), sticky="ew")

    def _build_roads_list_panel(self):
        card = ctk.CTkFrame(
            self, fg_color=COLORS["bg_card"], corner_radius=12,
            border_width=1, border_color=COLORS["border"]
        )
        card.grid(row=1, column=1, padx=(8, 16), pady=8, sticky="nsew")
        card.grid_columnconfigure(0, weight=1)
        card.grid_rowconfigure(2, weight=1)

        hdr = ctk.CTkFrame(card, fg_color="transparent")
        hdr.grid(row=0, column=0, padx=16, pady=(16, 4), sticky="ew")
        hdr.grid_columnconfigure(0, weight=1)

        ctk.CTkLabel(
            hdr, text="🛣️  Active Managed Roads",
            font=ctk.CTkFont(size=16, weight="bold"),
            text_color=COLORS["text_main"]
        ).grid(row=0, column=0, sticky="w")

        self.count_badge = ctk.CTkLabel(
            hdr, text="0 roads",
            font=ctk.CTkFont(size=12),
            text_color=COLORS["text_muted"]
        )
        self.count_badge.grid(row=0, column=1, sticky="e")

        ctk.CTkFrame(card, height=1, fg_color=COLORS["border"]).grid(
            row=1, column=0, padx=16, pady=(6, 8), sticky="ew"
        )

        self.roads_scroll = ctk.CTkScrollableFrame(
            card, fg_color="transparent",
            scrollbar_button_color=COLORS["border"],
            scrollbar_button_hover_color=COLORS["accent"]
        )
        self.roads_scroll.grid(row=2, column=0, padx=8, pady=(0, 8), sticky="nsew")
        self.roads_scroll.grid_columnconfigure(0, weight=1)

        ctk.CTkButton(
            card, text="🔄  Refresh Road List",
            fg_color=COLORS["bg_dark"], hover_color=COLORS["border"],
            border_color=COLORS["border"], border_width=1,
            text_color=COLORS["text_muted"],
            height=36, corner_radius=8,
            command=self.refresh_data
        ).grid(row=3, column=0, padx=16, pady=(4, 16), sticky="ew")

    def _render_road_item(self, idx, road):
        road_id = road.get("road_id", "")
        name = road.get("display_name", road_id)
        count = road.get("sensor_vehicle_count", 0)
        wait = road.get("max_waiting_time", 0)
        nq = road.get("normal_queue_size", 0)
        eq = road.get("emergency_queue_size", 0)

        level, color = get_traffic_level(count, wait)
        junction_link = ROAD_JUNCTIONS.get(road_id, "Connected Segment")

        item = ctk.CTkFrame(
            self.roads_scroll, fg_color=COLORS["bg_dark"], corner_radius=8,
            border_width=1, border_color=COLORS["border"]
        )
        item.grid(row=idx, column=0, sticky="ew", padx=6, pady=4)
        item.grid_columnconfigure(1, weight=1)

        ctk.CTkFrame(item, width=4, fg_color=color, corner_radius=4).grid(
            row=0, column=0, rowspan=3, padx=(6, 8), pady=8, sticky="ns"
        )

        title_box = ctk.CTkFrame(item, fg_color="transparent")
        title_box.grid(row=0, column=1, sticky="ew", padx=(0, 10), pady=(8, 2))
        title_box.grid_columnconfigure(0, weight=1)

        ctk.CTkLabel(
            title_box, text=f"{name} [{road_id}]",
            font=ctk.CTkFont(size=13, weight="bold"),
            text_color=COLORS["text_main"]
        ).grid(row=0, column=0, sticky="w")

        ctk.CTkLabel(
            title_box, text=level,
            font=ctk.CTkFont(size=10, weight="bold"),
            text_color=COLORS["bg_dark"], fg_color=color,
            corner_radius=4, width=55, height=20
        ).grid(row=0, column=1, sticky="e")

        ctk.CTkLabel(
            item, text=f"🔗 Junctions: {junction_link}",
            font=ctk.CTkFont(size=11), text_color=COLORS["accent"],
            anchor="w"
        ).grid(row=1, column=1, sticky="w", padx=(0, 10), pady=(0, 2))

        stats = f"Sensors: {count} vehicles | Wait: {wait}s | Normal Queue: {nq} | Emergency: {eq}"
        ctk.CTkLabel(
            item, text=stats,
            font=ctk.CTkFont(size=11), text_color=COLORS["text_muted"],
            anchor="w"
        ).grid(row=2, column=1, sticky="w", padx=(0, 10), pady=(0, 8))

    def add_road(self):
        if not self.app.backend or not self.app.backend.connected:
            self.app.log_activity("Backend not connected", False)
            return

        road_id = self.road_id_var.get().strip().upper()
        display_name = self.road_name_var.get().strip()
        junctions = self.junction_var.get().strip()

        if not road_id:
            self.app.log_activity("Road ID cannot be empty", False)
            return

        if not display_name:
            display_name = road_id.replace("_", " ").title()

        try:
            count = int(self.sensor_count_var.get())
            wait = int(self.max_wait_var.get())
            if count < 0 or wait < 0:
                raise ValueError
        except Exception:
            self.app.log_activity("Sensor count and wait time must be non-negative integers", False)
            return

        req = {
            "operation": "add_road",
            "road_id": road_id,
            "display_name": display_name,
            "sensor_count": count,
            "max_wait_time": wait
        }
        res = self.app.backend.send_request(req)
        if res and res.get("success"):
            if junctions:
                ROAD_JUNCTIONS[road_id] = junctions
            self.app.log_activity(f"Road '{display_name}' ({road_id}) registered successfully", True)
            self.road_id_var.set("")
            self.road_name_var.set("")
            self.refresh_data()
        else:
            msg = res.get("message", "Failed to add road") if res else "No response"
            self.app.log_activity(f"Error adding road: {msg}", False)

    def refresh_data(self):
        if not self.app.backend or not self.app.backend.connected:
            return

        res = self.app.backend.send_request({"operation": "get_roads"})
        if res and res.get("success"):
            self.roads_data = res["data"].get("roads", [])
            for w in self.roads_scroll.winfo_children():
                w.destroy()

            self.count_badge.configure(
                text=f"{len(self.roads_data)} {'road' if len(self.roads_data) == 1 else 'roads'}"
            )

            for idx, road in enumerate(self.roads_data):
                self._render_road_item(idx, road)

            self.app.log_activity(f"Road management refreshed ({len(self.roads_data)} roads)", True)
