import customtkinter as ctk
from theme import COLORS, get_traffic_level, get_green_duration

class TrafficAnalysisPage(ctk.CTkFrame):

    def __init__(self, master, app):
        super().__init__(master, fg_color="transparent")
        self.app = app
        self.road_cards = {}

        self.grid_columnconfigure(0, weight=1)
        self.grid_rowconfigure(1, weight=1)

        banner = ctk.CTkFrame(self, fg_color="#0D2137", corner_radius=8)
        banner.grid(row=0, column=0, sticky="ew", padx=4, pady=(0, 14))
        ctk.CTkLabel(
            banner,
            text="📊  Multi-Road Traffic Analysis — Live sensor evaluations and dynamic "
                 "routing cost weight multipliers across all active roads.",
            text_color=COLORS["accent"],
            font=ctk.CTkFont(size=12)
        ).pack(padx=16, pady=10)

        self.scroll = ctk.CTkScrollableFrame(self, fg_color="transparent")
        self.scroll.grid(row=1, column=0, sticky="nsew")
        self.scroll.grid_columnconfigure((0, 1), weight=1)

        self.roads_container = ctk.CTkFrame(self.scroll, fg_color="transparent")
        self.roads_container.grid(row=0, column=0, columnspan=2, sticky="nsew")
        self.roads_container.grid_columnconfigure((0, 1), weight=1)

        ref_card = ctk.CTkFrame(self.scroll, fg_color=COLORS["bg_card"], corner_radius=12)
        ref_card.grid(row=1, column=0, columnspan=2, sticky="ew", pady=(14, 0))
        self._build_reference_card(ref_card)

        ctk.CTkButton(self.scroll, text="🔄  Refresh Traffic Analysis",
                      fg_color=COLORS["accent"], hover_color="#1557C0",
                      height=38, corner_radius=8,
                      command=self.refresh_data).grid(
            row=2, column=0, columnspan=2, sticky="ew", pady=14)

    def _build_road_card(self, parent, title, road_id, col, row):
        card = ctk.CTkFrame(parent, fg_color=COLORS["bg_card"], corner_radius=12,
                            border_width=1, border_color=COLORS["border"])
        padx = (0, 7) if col == 0 else (7, 0)
        card.grid(row=row, column=col, padx=padx, pady=6, sticky="nsew")
        card.grid_columnconfigure(0, weight=1)

        ctk.CTkLabel(card, text=f"🛣  {title} ({road_id})",
                     font=ctk.CTkFont(size=14, weight="bold"),
                     text_color=COLORS["text_main"]).pack(anchor="w", padx=16, pady=(16, 4))
        ctk.CTkFrame(card, height=1, fg_color=COLORS["border"]).pack(fill="x", padx=16, pady=(0, 10))

        def row_label(label_text):
            f = ctk.CTkFrame(card, fg_color="transparent")
            f.pack(fill="x", padx=16, pady=2)
            ctk.CTkLabel(f, text=label_text,
                         text_color=COLORS["text_muted"],
                         font=ctk.CTkFont(size=12), width=150, anchor="w").pack(side="left")
            val = ctk.CTkLabel(f, text="—",
                               text_color=COLORS["text_main"],
                               font=ctk.CTkFont(size=12, weight="bold"), anchor="w")
            val.pack(side="left")
            return val

        labels = {}
        labels["count"]  = row_label("Vehicle Count:")
        labels["wait"]   = row_label("Max Wait Time:")
        labels["nq"]     = row_label("Normal Queue:")
        labels["eq"]     = row_label("Emergency Queue:")

        ctk.CTkFrame(card, height=1, fg_color=COLORS["border"]).pack(fill="x", padx=16, pady=(8, 8))

        f_level = ctk.CTkFrame(card, fg_color="transparent")
        f_level.pack(fill="x", padx=16, pady=(0, 6))
        ctk.CTkLabel(f_level, text="Traffic Level:",
                     text_color=COLORS["text_muted"],
                     font=ctk.CTkFont(size=12), width=150, anchor="w").pack(side="left")
        labels["level_badge"] = ctk.CTkLabel(f_level, text="—",
                                             font=ctk.CTkFont(size=12, weight="bold"),
                                             text_color=COLORS["green"])
        labels["level_badge"].pack(side="left")

        ctk.CTkFrame(card, height=1, fg_color=COLORS["border"]).pack(fill="x", padx=16, pady=(6, 8))
        ctk.CTkLabel(card, text="Weight Multiplier (Routing Engine):",
                     text_color=COLORS["text_muted"],
                     font=ctk.CTkFont(size=11)).pack(anchor="w", padx=16)
        labels["multiplier"] = ctk.CTkLabel(card, text="—",
                                            font=ctk.CTkFont(size=15, weight="bold"),
                                            text_color=COLORS["text_main"])
        labels["multiplier"].pack(anchor="w", padx=16, pady=(2, 14))

        card.labels = labels
        return card

    def _build_reference_card(self, parent):
        ctk.CTkLabel(parent, text="Traffic Classification & Weight Multiplier Model",
                     font=ctk.CTkFont(size=14, weight="bold"),
                     text_color=COLORS["text_main"]).pack(anchor="w", padx=16, pady=(16, 8))
        ctk.CTkFrame(parent, height=1, fg_color=COLORS["border"]).pack(fill="x", padx=16, pady=(0, 12))

        rows_frame = ctk.CTkFrame(parent, fg_color="transparent")
        rows_frame.pack(fill="x", padx=16, pady=(0, 16))
        rows_frame.grid_columnconfigure((0, 1, 2, 3), weight=1)

        headers = ["Level", "Threshold Condition", "Weight Multiplier", "Adaptive Green Light"]
        for col, h in enumerate(headers):
            ctk.CTkLabel(rows_frame, text=h,
                         font=ctk.CTkFont(size=12, weight="bold"),
                         text_color=COLORS["text_muted"]).grid(row=0, column=col, padx=8, pady=4, sticky="w")

        data = [
            ("🟢  LOW",    "≤ 5 vehicles AND ≤ 30s", "× 1.0  (base weight)",   "20 seconds"),
            ("🟡  MEDIUM", "≤ 15 vehicles AND ≤ 60s", "× 1.5  (+50% penalty)", "35 seconds"),
            ("🔴  HIGH",   "> 15 vehicles OR > 60s",  "× 2.5  (+150% penalty)", "50 seconds"),
            ("🚨  EMERGENCY", "Any emergency vehicle", "Immediate signal preempt", "60 seconds"),
        ]
        colors = [COLORS["green"], COLORS["yellow"], COLORS["red"], COLORS["red"]]
        for r, (row_data, color) in enumerate(zip(data, colors), start=1):
            for col, cell in enumerate(row_data):
                ctk.CTkLabel(rows_frame, text=cell,
                             font=ctk.CTkFont(size=12),
                             text_color=color if col == 0 else COLORS["text_main"]).grid(
                    row=r, column=col, padx=8, pady=3, sticky="w")

        ctk.CTkLabel(parent,
                     text="ℹ️  Dynamic weight multipliers adjust edge costs dynamically inside the C++ Graph engine. "
                          "Adaptive green lights prioritize roads experiencing higher traffic congestion.",
                     text_color=COLORS["text_muted"],
                     font=ctk.CTkFont(size=11),
                     wraplength=800, justify="left").pack(anchor="w", padx=16, pady=(0, 16))

    def _update_card(self, card, data):
        count = data.get("sensor_vehicle_count", 0)
        wait  = data.get("max_waiting_time", 0)
        nq    = data.get("normal_queue_size", 0)
        eq    = data.get("emergency_queue_size", 0)

        level, color = get_traffic_level(count, wait)
        multipliers = {"LOW": "× 1.0  (no change)", "MEDIUM": "× 1.5  (+50%)", "HIGH": "× 2.5  (+150%)"}

        card.labels["count"].configure(text=str(count))
        card.labels["wait"].configure(text=f"{wait}s")
        card.labels["nq"].configure(text=str(nq))
        card.labels["eq"].configure(text=str(eq))
        card.labels["level_badge"].configure(text=level, text_color=color)
        card.labels["multiplier"].configure(text=multipliers.get(level, "× 1.0"), text_color=color)

    def refresh_data(self):
        if not self.app.backend or not self.app.backend.connected:
            return

        res = self.app.backend.send_request({"operation": "get_roads"})
        if not res or not res.get("success"):
            return

        roads = res["data"].get("roads", [])

        for w in self.roads_container.winfo_children():
            w.destroy()
        self.road_cards.clear()

        for idx, r in enumerate(roads):
            col = idx % 2
            row = idx // 2
            road_id = r.get("road_id", "")
            name = r.get("display_name", road_id)
            card = self._build_road_card(self.roads_container, name, road_id, col, row)
            self._update_card(card, r)
            self.road_cards[road_id] = card

        self.app.log_activity(f"Traffic analysis updated for {len(roads)} road(s)", True)
