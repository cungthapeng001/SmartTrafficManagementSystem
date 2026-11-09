import customtkinter as ctk
from theme import COLORS

BG_DARK    = COLORS["bg_dark"]
BG_CARD    = COLORS["bg_card"]
ACCENT     = COLORS["accent"]
TEXT_MAIN  = COLORS["text_main"]
TEXT_MUTED = COLORS["text_muted"]
GREEN      = COLORS["green"]
YELLOW     = COLORS["yellow"]
RED        = COLORS["red"]
BORDER     = COLORS["border"]


def traffic_badge_color(sensor_count):
    if sensor_count <= 5:
        return "LOW", GREEN
    elif sensor_count <= 15:
        return "MEDIUM", YELLOW
    else:
        return "HIGH", RED


class RoadSensorsPage(ctk.CTkFrame):
    """
    Road Sensors page.
    Left card: form to update simulated sensor data for any selected road.
    Right card: scrollable view of current sensor readings for all roads.
    """

    def __init__(self, master, app):
        super().__init__(master, fg_color="transparent")
        self.app = app
        self.available_roads = ["NORTH", "SOUTH", "EAST", "WEST", "CENTRAL"]
        self.mini_cards = {}  # road_id -> dict of label widgets

        self.grid_columnconfigure((0, 1), weight=1)
        self.grid_rowconfigure(0, weight=1)

        # ── Left: Update form ───────────────────────────────────────────────
        self._build_form_card()

        # ── Right: Current status display ───────────────────────────────────
        self._build_status_card()

    # ── Update form card ───────────────────────────────────────────────────────

    def _build_form_card(self):
        """Left-side card with inputs for updating sensor simulation data."""
        self.form_frame = ctk.CTkFrame(
            self, fg_color=BG_CARD, corner_radius=12,
            border_width=1, border_color=BORDER
        )
        self.form_frame.grid(row=0, column=0, padx=(16, 8), pady=16, sticky="nsew")
        self.form_frame.grid_columnconfigure(0, weight=1)

        # Card title
        ctk.CTkLabel(
            self.form_frame,
            text="Update Simulated Sensors",
            font=ctk.CTkFont(size=16, weight="bold"),
            text_color=TEXT_MAIN
        ).grid(row=0, column=0, padx=16, pady=(20, 4), sticky="w")

        ctk.CTkLabel(
            self.form_frame,
            text="Set simulated sensor counts and wait times for any road.",
            font=ctk.CTkFont(size=11),
            text_color=TEXT_MUTED,
            wraplength=280,
            justify="left"
        ).grid(row=1, column=0, padx=16, pady=(0, 12), sticky="w")

        ctk.CTkFrame(self.form_frame, height=1, fg_color=BORDER).grid(
            row=2, column=0, padx=16, pady=(0, 16), sticky="ew"
        )

        # Road selection
        self.road_var = ctk.StringVar(value="NORTH")
        self._form_label(self.form_frame, "Road Selection:", 3)
        self.road_menu = ctk.CTkOptionMenu(
            self.form_frame,
            variable=self.road_var,
            values=self.available_roads,
            fg_color=BG_DARK,
            button_color=ACCENT,
            button_hover_color="#1558c0",
            text_color=TEXT_MAIN,
            corner_radius=8
        )
        self.road_menu.grid(row=4, column=0, padx=16, pady=(0, 14), sticky="ew")

        # Vehicle count
        self.count_var = ctk.StringVar(value="10")
        self._form_label(self.form_frame, "Simulated Vehicle Count:", 5)
        ctk.CTkEntry(
            self.form_frame,
            textvariable=self.count_var,
            fg_color=BG_DARK,
            border_color=BORDER,
            text_color=TEXT_MAIN,
            placeholder_text="e.g. 10",
            corner_radius=8
        ).grid(row=6, column=0, padx=16, pady=(0, 14), sticky="ew")

        # Max wait time
        self.wait_var = ctk.StringVar(value="30")
        self._form_label(self.form_frame, "Simulated Max Wait Time (s):", 7)
        ctk.CTkEntry(
            self.form_frame,
            textvariable=self.wait_var,
            fg_color=BG_DARK,
            border_color=BORDER,
            text_color=TEXT_MAIN,
            placeholder_text="e.g. 60",
            corner_radius=8
        ).grid(row=8, column=0, padx=16, pady=(0, 20), sticky="ew")

        # Submit button
        ctk.CTkButton(
            self.form_frame,
            text="Update Sensors for Selected Road",
            font=ctk.CTkFont(size=14, weight="bold"),
            fg_color=ACCENT,
            hover_color="#1558c0",
            corner_radius=8,
            height=40,
            command=self.update_sensors
        ).grid(row=9, column=0, padx=16, pady=(0, 24), sticky="ew")

    def _form_label(self, parent, text, row):
        ctk.CTkLabel(
            parent,
            text=text,
            font=ctk.CTkFont(size=13),
            text_color=TEXT_MUTED,
            anchor="w"
        ).grid(row=row, column=0, padx=16, pady=(0, 4), sticky="w")

    # ── Status display card ────────────────────────────────────────────────────

    def _build_status_card(self):
        """Right-side card showing scrollable sensor readings for all roads."""
        self.status_frame = ctk.CTkFrame(
            self, fg_color=BG_CARD, corner_radius=12,
            border_width=1, border_color=BORDER
        )
        self.status_frame.grid(row=0, column=1, padx=(8, 16), pady=16, sticky="nsew")
        self.status_frame.grid_columnconfigure(0, weight=1)
        self.status_frame.grid_rowconfigure(3, weight=1)

        # Card title
        ctk.CTkLabel(
            self.status_frame,
            text="Current Sensor Status",
            font=ctk.CTkFont(size=16, weight="bold"),
            text_color=TEXT_MAIN
        ).grid(row=0, column=0, padx=16, pady=(20, 4), sticky="w")

        ctk.CTkLabel(
            self.status_frame,
            text="Live sensor readings and queue state across all roads.",
            font=ctk.CTkFont(size=11),
            text_color=TEXT_MUTED
        ).grid(row=1, column=0, padx=16, pady=(0, 10), sticky="w")

        ctk.CTkFrame(self.status_frame, height=1, fg_color=BORDER).grid(
            row=2, column=0, padx=16, pady=(0, 8), sticky="ew"
        )

        # Scrollable container for road mini-cards
        self.sensor_scroll = ctk.CTkScrollableFrame(
            self.status_frame,
            fg_color="transparent",
            scrollbar_button_color=BORDER,
            scrollbar_button_hover_color=ACCENT
        )
        self.sensor_scroll.grid(row=3, column=0, padx=8, pady=(0, 8), sticky="nsew")
        self.sensor_scroll.grid_columnconfigure(0, weight=1)

        # Refresh button
        ctk.CTkButton(
            self.status_frame,
            text="🔄  Refresh All Sensors",
            fg_color=BG_DARK,
            hover_color=BORDER,
            border_color=BORDER,
            border_width=1,
            text_color=TEXT_MUTED,
            corner_radius=8,
            font=ctk.CTkFont(size=13),
            height=38,
            command=self.refresh_data
        ).grid(row=4, column=0, padx=16, pady=16, sticky="ew")

    def _render_road_sensor_card(self, parent, road_info, row):
        road_id = road_info.get("road_id", "")
        display_name = road_info.get("display_name", road_id)
        count = road_info.get("sensor_vehicle_count", 0)
        wait = road_info.get("max_waiting_time", 0)
        nq = road_info.get("normal_queue_size", 0)
        eq = road_info.get("emergency_queue_size", 0)

        badge_text, badge_color = traffic_badge_color(count)

        card = ctk.CTkFrame(parent, fg_color=BG_DARK, corner_radius=10,
                            border_width=1, border_color=BORDER)
        card.grid(row=row, column=0, padx=8, pady=4, sticky="ew")
        card.grid_columnconfigure(0, weight=1)

        hdr = ctk.CTkFrame(card, fg_color="transparent")
        hdr.grid(row=0, column=0, padx=12, pady=(10, 4), sticky="ew")
        hdr.grid_columnconfigure(0, weight=1)

        ctk.CTkLabel(hdr, text=f"🛣  {display_name} ({road_id})",
                     font=ctk.CTkFont(size=13, weight="bold"),
                     text_color=TEXT_MAIN).grid(row=0, column=0, sticky="w")

        ctk.CTkLabel(hdr, text=badge_text,
                     font=ctk.CTkFont(size=10, weight="bold"),
                     text_color=BG_DARK, fg_color=badge_color,
                     corner_radius=6, width=65).grid(row=0, column=1, sticky="e")

        def make_row(r, label_text, val_text):
            rf = ctk.CTkFrame(card, fg_color="transparent")
            rf.grid(row=r, column=0, padx=12, pady=1, sticky="ew")
            rf.grid_columnconfigure(1, weight=1)
            ctk.CTkLabel(rf, text=label_text, font=ctk.CTkFont(size=12),
                         text_color=TEXT_MUTED).grid(row=0, column=0, sticky="w")
            ctk.CTkLabel(rf, text=val_text, font=ctk.CTkFont(size=12, weight="bold"),
                         text_color=TEXT_MAIN).grid(row=0, column=1, sticky="e")

        make_row(1, "Sensor Count:", f"{count} vehicles")
        make_row(2, "Max Wait Time:", f"{wait}s")
        make_row(3, "Queues (Normal / Emergency):", f"{nq} / {eq}")

        ctk.CTkFrame(card, height=6, fg_color="transparent").grid(row=4, column=0)
        return card

    # ── Backend operations ─────────────────────────────────────────────────────

    def update_road_list(self):
        if not self.app.backend or not self.app.backend.connected:
            return
        res = self.app.backend.send_request({"operation": "get_roads"})
        if res and res.get("success"):
            roads = [r["road_id"] for r in res["data"].get("roads", [])]
            if roads:
                self.available_roads = roads
                self.road_menu.configure(values=self.available_roads)
                if self.road_var.get() not in self.available_roads:
                    self.road_var.set(self.available_roads[0])

    def update_sensors(self):
        if not self.app.backend or not self.app.backend.connected:
            return

        try:
            count = int(self.count_var.get())
            wait  = int(self.wait_var.get())
            if count < 0 or wait < 0:
                raise ValueError
        except Exception:
            self.app.log_activity("Invalid sensor values (must be non-negative integers).", False)
            return

        req = {
            "operation":    "update_sensor_info",
            "road_id":      self.road_var.get(),
            "sensor_count": count,
            "max_wait_time": wait
        }
        res = self.app.backend.send_request(req)
        self.app.log_activity(res.get("message", "Error"), res.get("success", False))
        if res.get("success"):
            self.refresh_data()

    def refresh_data(self):
        if not self.app.backend or not self.app.backend.connected:
            return

        self.update_road_list()

        res = self.app.backend.send_request({"operation": "get_roads"})
        if res and res.get("success"):
            roads = res["data"].get("roads", [])
            for w in self.sensor_scroll.winfo_children():
                w.destroy()

            for idx, r in enumerate(roads):
                self._render_road_sensor_card(self.sensor_scroll, r, idx)

            self.app.log_activity(f"Road sensor status refreshed for {len(roads)} road(s)", True)
