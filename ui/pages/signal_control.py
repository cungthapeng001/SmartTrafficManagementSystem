import customtkinter as ctk
from theme import COLORS, get_traffic_level, get_green_duration

class SignalControlPage(ctk.CTkFrame):

    def __init__(self, master, app):
        super().__init__(master, fg_color="transparent")
        self.app = app
        self.available_roads = ["NORTH", "SOUTH", "EAST", "WEST", "CENTRAL"]

        self.grid_columnconfigure(0, weight=1)
        self.grid_rowconfigure(2, weight=1)

        banner = ctk.CTkFrame(self, fg_color="#0D2137", corner_radius=8)
        banner.grid(row=0, column=0, sticky="ew", padx=4, pady=(0, 10))
        ctk.CTkLabel(
            banner,
            text="🚦  Adaptive Signal Simulation — Dynamic intersection control comparing road traffic demands "
                 "and applying emergency preemption rules.",
            text_color=COLORS["accent"],
            font=ctk.CTkFont(size=12)
        ).pack(padx=16, pady=10)

        self.emerg_banner = ctk.CTkFrame(self, fg_color="#2D0000", corner_radius=8)
        self.emerg_banner_lbl = ctk.CTkLabel(
            self.emerg_banner,
            text="🚨  EMERGENCY VEHICLE DETECTED — Priority Signal Preemption Active!",
            text_color=COLORS["red"],
            font=ctk.CTkFont(size=13, weight="bold")
        )
        self.emerg_banner_lbl.pack(padx=16, pady=10)

        road_row = ctk.CTkFrame(self, fg_color="transparent")
        road_row.grid(row=2, column=0, sticky="nsew", padx=4)
        road_row.grid_columnconfigure(0, weight=1)
        road_row.grid_columnconfigure(1, weight=1)
        road_row.grid_rowconfigure(0, weight=1)

        self.road1_var = ctk.StringVar(value="NORTH")
        self.road2_var = ctk.StringVar(value="SOUTH")

        self.panel1 = self._build_road_panel(road_row, self.road1_var, 0)
        self.panel2 = self._build_road_panel(road_row, self.road2_var, 1)

        decision_card = ctk.CTkFrame(
            self, fg_color=COLORS["bg_card"], corner_radius=12,
            border_width=1, border_color=COLORS["border"]
        )
        decision_card.grid(row=3, column=0, sticky="ew", padx=4, pady=(10, 0))
        decision_card.grid_columnconfigure(0, weight=1)
        self._build_decision_panel(decision_card)

        btn_row = ctk.CTkFrame(self, fg_color="transparent")
        btn_row.grid(row=4, column=0, sticky="ew", padx=4, pady=12)

        ctk.CTkButton(
            btn_row,
            text="▶  Run Signal Simulation",
            fg_color=COLORS["accent"],
            hover_color="#1557C0",
            font=ctk.CTkFont(size=14, weight="bold"),
            height=40,
            corner_radius=8,
            command=self.run_simulation
        ).pack(side="left", padx=(0, 10))

        ctk.CTkButton(
            btn_row,
            text="🔄  Refresh Road Data",
            fg_color=COLORS["bg_card"],
            hover_color=COLORS["border"],
            border_color=COLORS["border"],
            border_width=1,
            text_color=COLORS["text_muted"],
            font=ctk.CTkFont(size=13),
            height=40,
            corner_radius=8,
            command=self.refresh_data
        ).pack(side="left")

    def _build_road_panel(self, parent, road_var, col):
        padx = (0, 7) if col == 0 else (7, 0)
        card = ctk.CTkFrame(
            parent, fg_color=COLORS["bg_card"], corner_radius=12,
            border_width=1, border_color=COLORS["border"]
        )
        card.grid(row=0, column=col, padx=padx, sticky="nsew")
        card.grid_columnconfigure(0, weight=1)

        hdr = ctk.CTkFrame(card, fg_color="transparent")
        hdr.grid(row=0, column=0, padx=16, pady=(14, 4), sticky="ew")
        hdr.grid_columnconfigure(1, weight=1)

        ctk.CTkLabel(
            hdr, text=f"Approach {col + 1}:",
            font=ctk.CTkFont(size=14, weight="bold"),
            text_color=COLORS["text_main"]
        ).grid(row=0, column=0, sticky="w", padx=(0, 8))

        menu = ctk.CTkOptionMenu(
            hdr,
            variable=road_var,
            values=self.available_roads,
            fg_color=COLORS["bg_dark"],
            button_color=COLORS["accent"],
            text_color=COLORS["text_main"],
            corner_radius=8,
            command=lambda _: self.refresh_data()
        )
        menu.grid(row=0, column=1, sticky="ew")

        ctk.CTkFrame(card, height=1, fg_color=COLORS["border"]).grid(
            row=1, column=0, padx=16, pady=(10, 8), sticky="ew"
        )

        signal_circle = ctk.CTkLabel(
            card, text="⬤", font=ctk.CTkFont(size=36),
            text_color=COLORS["border"]
        )
        signal_circle.grid(row=2, column=0, padx=16, pady=(2, 2))

        signal_state = ctk.CTkLabel(
            card, text="READY",
            font=ctk.CTkFont(size=12, weight="bold"),
            text_color=COLORS["text_muted"]
        )
        signal_state.grid(row=3, column=0, padx=16, pady=(0, 8))

        ctk.CTkFrame(card, height=1, fg_color=COLORS["border"]).grid(
            row=4, column=0, padx=16, pady=(0, 8), sticky="ew"
        )

        def make_row(label_text, row_idx):
            f = ctk.CTkFrame(card, fg_color="transparent")
            f.grid(row=row_idx, column=0, padx=16, pady=2, sticky="ew")
            f.grid_columnconfigure(1, weight=1)
            ctk.CTkLabel(f, text=label_text, font=ctk.CTkFont(size=12),
                         text_color=COLORS["text_muted"], anchor="w", width=150).grid(row=0, column=0, sticky="w")
            val = ctk.CTkLabel(f, text="—", font=ctk.CTkFont(size=12, weight="bold"),
                               text_color=COLORS["text_main"])
            val.grid(row=0, column=1, sticky="e")
            return val

        lbl_count = make_row("Sensor Count:",      5)
        lbl_wait  = make_row("Max Wait Time:",     6)
        lbl_nq    = make_row("Normal Queue:",      7)
        lbl_eq    = make_row("Emergency Queue:",   8)

        ctk.CTkFrame(card, height=1, fg_color=COLORS["border"]).grid(
            row=9, column=0, padx=16, pady=(8, 6), sticky="ew"
        )

        badge_row = ctk.CTkFrame(card, fg_color="transparent")
        badge_row.grid(row=10, column=0, padx=16, pady=(0, 6), sticky="ew")
        badge_row.grid_columnconfigure(1, weight=1)
        ctk.CTkLabel(badge_row, text="Traffic Level:", font=ctk.CTkFont(size=12),
                     text_color=COLORS["text_muted"], anchor="w", width=150).grid(row=0, column=0, sticky="w")
        level_badge = ctk.CTkLabel(
            badge_row, text="—", font=ctk.CTkFont(size=11, weight="bold"),
            text_color=COLORS["bg_dark"], fg_color=COLORS["text_muted"],
            corner_radius=6, width=70
        )
        level_badge.grid(row=0, column=1, sticky="e")

        ctk.CTkFrame(card, height=1, fg_color=COLORS["border"]).grid(
            row=11, column=0, padx=16, pady=(0, 6), sticky="ew"
        )

        ctk.CTkLabel(card, text="Adaptive Green Light:", font=ctk.CTkFont(size=11),
                     text_color=COLORS["text_muted"]).grid(row=12, column=0, padx=16, sticky="w")
        duration_lbl = ctk.CTkLabel(
            card, text="—", font=ctk.CTkFont(size=20, weight="bold"),
            text_color=COLORS["green"]
        )
        duration_lbl.grid(row=13, column=0, padx=16, pady=(2, 12), sticky="w")

        card.labels = {
            "count":    lbl_count,
            "wait":     lbl_wait,
            "nq":       lbl_nq,
            "eq":       lbl_eq,
            "badge":    level_badge,
            "circle":   signal_circle,
            "state":    signal_state,
            "duration": duration_lbl,
            "menu":     menu,
        }
        card.road_var = road_var
        return card

    def _build_decision_panel(self, parent):
        ctk.CTkLabel(
            parent, text="🚦  Adaptive Controller Decision",
            font=ctk.CTkFont(size=14, weight="bold"),
            text_color=COLORS["text_main"]
        ).grid(row=0, column=0, padx=16, pady=(14, 4), sticky="w")

        ctk.CTkFrame(parent, height=1, fg_color=COLORS["border"]).grid(
            row=1, column=0, padx=16, pady=(0, 10), sticky="ew"
        )

        self.decision_lbl = ctk.CTkLabel(
            parent,
            text="Press 'Run Signal Simulation' to compute adaptive green light allocation.",
            font=ctk.CTkFont(size=12),
            text_color=COLORS["text_muted"],
            justify="left",
            wraplength=880,
            anchor="w"
        )
        self.decision_lbl.grid(row=2, column=0, padx=16, pady=(0, 10), sticky="ew")

        ctk.CTkLabel(
            parent,
            text="ℹ️  Signal arbitration uses the C++ TrafficSignal logic: emergency preemption overrides all normal rules, "
                 "followed by highest traffic condition level and vehicle count tie-break.",
            font=ctk.CTkFont(size=11),
            text_color=COLORS["text_muted"],
            justify="left",
            wraplength=880
        ).grid(row=3, column=0, padx=16, pady=(0, 12), sticky="ew")

    def _update_panel(self, panel, data):
        count = data.get("sensor_vehicle_count", 0)
        wait  = data.get("max_waiting_time", 0)
        nq    = data.get("normal_queue_size", 0)
        eq    = data.get("emergency_queue_size", 0)

        level, level_color = get_traffic_level(count, wait)
        duration = get_green_duration(level)

        labels = panel.labels
        labels["count"].configure(text=str(count))
        labels["wait"].configure(text=f"{wait}s")
        labels["nq"].configure(text=str(nq))
        labels["eq"].configure(text=str(eq))
        labels["badge"].configure(text=level, fg_color=level_color, text_color=COLORS["bg_dark"])
        labels["duration"].configure(text=f"{duration} seconds", text_color=level_color)

        panel.level    = level
        panel.color    = level_color
        panel.duration = duration
        panel.eq       = eq
        panel.count    = count

        labels["circle"].configure(text_color=COLORS["border"])
        labels["state"].configure(text="READY", text_color=COLORS["text_muted"])

    def update_road_list(self):
        if not self.app.backend or not self.app.backend.connected:
            return
        res = self.app.backend.send_request({"operation": "get_roads"})
        if res and res.get("success"):
            roads = [r["road_id"] for r in res["data"].get("roads", [])]
            if roads:
                self.available_roads = roads
                self.panel1.labels["menu"].configure(values=self.available_roads)
                self.panel2.labels["menu"].configure(values=self.available_roads)
                if self.road1_var.get() not in self.available_roads:
                    self.road1_var.set(self.available_roads[0])
                if self.road2_var.get() not in self.available_roads:
                    self.road2_var.set(self.available_roads[1] if len(self.available_roads) > 1 else self.available_roads[0])

    def refresh_data(self):
        if not self.app.backend or not self.app.backend.connected:
            return

        self.update_road_list()

        r1 = self.road1_var.get()
        r2 = self.road2_var.get()

        res1 = self.app.backend.send_request({"operation": "get_traffic_info", "road_id": r1})
        res2 = self.app.backend.send_request({"operation": "get_traffic_info", "road_id": r2})

        if res1 and res1.get("success"):
            self._update_panel(self.panel1, res1["data"])
        if res2 and res2.get("success"):
            self._update_panel(self.panel2, res2["data"])

        eq1 = getattr(self.panel1, "eq", 0)
        eq2 = getattr(self.panel2, "eq", 0)
        if eq1 > 0 or eq2 > 0:
            self.emerg_banner.grid(row=1, column=0, sticky="ew", padx=4, pady=(0, 10))
        else:
            self.emerg_banner.grid_forget()

        self.app.log_activity(f"Signal controller data refreshed for {r1} & {r2}", True)

    def run_simulation(self):
        self.refresh_data()

        eq1 = getattr(self.panel1, "eq", 0)
        eq2 = getattr(self.panel2, "eq", 0)
        r1_name = self.road1_var.get()
        r2_name = self.road2_var.get()

        if eq1 > 0 or eq2 > 0:
            if eq1 > 0:
                winner_panel, loser_panel = self.panel1, self.panel2
                winner_name = r1_name
                eq_count = eq1
            else:
                winner_panel, loser_panel = self.panel2, self.panel1
                winner_name = r2_name
                eq_count = eq2

            winner_panel.labels["circle"].configure(text_color=COLORS["red"])
            winner_panel.labels["state"].configure(text="🚨 EMERGENCY 60s", text_color=COLORS["red"])
            loser_panel.labels["circle"].configure(text_color=COLORS["red"])
            loser_panel.labels["state"].configure(text="RED — WAIT", text_color=COLORS["red"])

            reason = (f"🚨  EMERGENCY PREEMPTION ACTIVE on {winner_name} ({eq_count} emergency vehicle(s)). "
                      f"Immediate 60-second priority green light allocated.")
            self.decision_lbl.configure(text=reason, text_color=COLORS["red"])
            self.app.log_activity(f"Adaptive signal: Emergency preemption -> {winner_name}", True)
            return

        level_score = {"HIGH": 3, "MEDIUM": 2, "LOW": 1}
        l1 = getattr(self.panel1, "level", "LOW")
        l2 = getattr(self.panel2, "level", "LOW")
        s1 = level_score.get(l1, 1)
        s2 = level_score.get(l2, 1)
        c1 = getattr(self.panel1, "count", 0)
        c2 = getattr(self.panel2, "count", 0)

        if s1 > s2:
            winner_panel, loser_panel = self.panel1, self.panel2
            winner_name, loser_name = r1_name, r2_name
            w_level, w_dur = l1, self.panel1.duration
            reason = f"✅  {winner_name} wins green light ({w_dur}s) — Traffic level {l1} exceeds {loser_name} ({l2})."
        elif s2 > s1:
            winner_panel, loser_panel = self.panel2, self.panel1
            winner_name, loser_name = r2_name, r1_name
            w_level, w_dur = l2, self.panel2.duration
            reason = f"✅  {winner_name} wins green light ({w_dur}s) — Traffic level {l2} exceeds {loser_name} ({l1})."
        elif c1 >= c2:
            winner_panel, loser_panel = self.panel1, self.panel2
            winner_name = r1_name
            w_dur = self.panel1.duration
            reason = f"✅  {winner_name} wins green light ({w_dur}s) — Equal traffic level ({l1}), tie-break on sensor count ({c1} vs {c2})."
        else:
            winner_panel, loser_panel = self.panel2, self.panel1
            winner_name = r2_name
            w_dur = self.panel2.duration
            reason = f"✅  {winner_name} wins green light ({w_dur}s) — Equal traffic level ({l2}), tie-break on sensor count ({c2} vs {c1})."

        winner_panel.labels["circle"].configure(text_color=COLORS["green"])
        winner_panel.labels["state"].configure(text=f"GREEN {w_dur}s", text_color=COLORS["green"])
        loser_panel.labels["circle"].configure(text_color=COLORS["red"])
        loser_panel.labels["state"].configure(text="RED — WAIT", text_color=COLORS["red"])

        self.decision_lbl.configure(text=reason, text_color=COLORS["green"])
        self.app.log_activity(f"Adaptive signal: {winner_name} allocated {w_dur}s green light", True)
