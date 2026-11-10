import customtkinter as ctk

BG_DARK    = "#0D1117"
BG_CARD    = "#161B22"
BG_ROW_ALT = "#1C2128"
ACCENT     = "#1F6FEB"
TEXT_MAIN  = "#E6EDF3"
TEXT_MUTED = "#8B949E"
GREEN      = "#2EA043"
YELLOW     = "#D29922"
RED        = "#DA3633"
BORDER     = "#30363D"

def traffic_badge(sensor_count):
    if sensor_count <= 5:
        return "LOW", GREEN
    elif sensor_count <= 15:
        return "MEDIUM", YELLOW
    else:
        return "HIGH", RED

class DashboardPage(ctk.CTkFrame):

    def __init__(self, master, app):
        super().__init__(master, fg_color="transparent")
        self.app = app
        self.road_card_frames = {}

        self.grid_columnconfigure(0, weight=1)
        self.grid_rowconfigure(1, weight=1)

        self._build_summary_row()

        self._build_roads_area()

        self._build_action_row()

    def _build_summary_row(self):
        summary_frame = ctk.CTkFrame(self, fg_color="transparent")
        summary_frame.grid(row=0, column=0, sticky="ew", padx=16, pady=(16, 8))
        summary_frame.grid_columnconfigure((0, 1, 2, 3), weight=1)

        self.total_vehicles_lbl = self._make_stat_card(summary_frame, "Total Vehicles",   "0", TEXT_MAIN, 0)
        self.normal_queue_lbl   = self._make_stat_card(summary_frame, "Normal Queue",      "0", GREEN,     1)
        self.emerg_queue_lbl    = self._make_stat_card(summary_frame, "Emergency Queue",   "0", RED,       2)
        self.active_roads_lbl   = self._make_stat_card(summary_frame, "Active Roads",      "0", ACCENT,    3)

    def _make_stat_card(self, parent, title, initial_val, value_color, col):
        card = ctk.CTkFrame(parent, fg_color=BG_CARD, corner_radius=12,
                            border_width=1, border_color=BORDER)
        card.grid(row=0, column=col, padx=6, pady=6, sticky="nsew")
        card.grid_columnconfigure(0, weight=1)

        ctk.CTkLabel(card, text=title,
                     font=ctk.CTkFont(size=11), text_color=TEXT_MUTED
                     ).grid(row=0, column=0, padx=16, pady=(14, 2), sticky="w")

        val_lbl = ctk.CTkLabel(card, text=initial_val,
                               font=ctk.CTkFont(size=28, weight="bold"),
                               text_color=value_color)
        val_lbl.grid(row=1, column=0, padx=16, pady=(0, 14), sticky="w")
        return val_lbl

    def _build_roads_area(self):
        outer = ctk.CTkFrame(self, fg_color=BG_CARD, corner_radius=12,
                             border_width=1, border_color=BORDER)
        outer.grid(row=1, column=0, sticky="nsew", padx=16, pady=8)
        outer.grid_columnconfigure(0, weight=1)
        outer.grid_rowconfigure(1, weight=1)

        hdr = ctk.CTkFrame(outer, fg_color="transparent")
        hdr.grid(row=0, column=0, sticky="ew", padx=16, pady=(14, 4))
        hdr.grid_columnconfigure(0, weight=1)

        ctk.CTkLabel(hdr, text="Multi-Road Overview",
                     font=ctk.CTkFont(size=16, weight="bold"),
                     text_color=TEXT_MAIN).grid(row=0, column=0, sticky="w")

        self.scroll_area = ctk.CTkScrollableFrame(
            outer, fg_color="transparent",
            scrollbar_button_color=BORDER,
            scrollbar_button_hover_color=ACCENT
        )
        self.scroll_area.grid(row=1, column=0, sticky="nsew", padx=8, pady=(0, 8))
        self.scroll_area.grid_columnconfigure((0, 1), weight=1)

    def _build_road_card(self, parent, road_id, display_name, data, card_num):
        col = card_num % 2
        row = card_num // 2

        card = ctk.CTkFrame(parent, fg_color=BG_DARK, corner_radius=10,
                            border_width=1, border_color=BORDER)
        card.grid(row=row, column=col,
                  padx=(0, 7) if col == 0 else (7, 0),
                  pady=6, sticky="nsew")
        card.grid_columnconfigure(0, weight=1)

        hdr = ctk.CTkFrame(card, fg_color="transparent")
        hdr.grid(row=0, column=0, padx=14, pady=(12, 4), sticky="ew")
        hdr.grid_columnconfigure(0, weight=1)

        ctk.CTkLabel(hdr, text=f"🛣  {display_name}",
                     font=ctk.CTkFont(size=14, weight="bold"),
                     text_color=TEXT_MAIN).grid(row=0, column=0, sticky="w")

        count = data.get("sensor_vehicle_count", 0)
        badge_text, badge_color = traffic_badge(count)
        badge_lbl = ctk.CTkLabel(hdr, text=badge_text,
                                 font=ctk.CTkFont(size=10, weight="bold"),
                                 text_color=BG_DARK, fg_color=badge_color,
                                 corner_radius=6, width=60)
        badge_lbl.grid(row=0, column=1, sticky="e")

        ctk.CTkFrame(card, height=1, fg_color=BORDER).grid(
            row=1, column=0, padx=14, pady=(0, 6), sticky="ew")

        def make_row(r, label_text, value_text):
            f = ctk.CTkFrame(card, fg_color="transparent")
            f.grid(row=r, column=0, padx=14, pady=2, sticky="ew")
            f.grid_columnconfigure(1, weight=1)
            ctk.CTkLabel(f, text=label_text, font=ctk.CTkFont(size=12),
                         text_color=TEXT_MUTED, anchor="w").grid(row=0, column=0, sticky="w")
            val = ctk.CTkLabel(f, text=value_text,
                               font=ctk.CTkFont(size=12, weight="bold"),
                               text_color=TEXT_MAIN)
            val.grid(row=0, column=1, sticky="e")
            return val

        lbl_sensor = make_row(2, "Sensor Count:",    str(data.get("sensor_vehicle_count", 0)))
        lbl_wait   = make_row(3, "Max Wait:",        f"{data.get('max_waiting_time', 0)}s")
        lbl_nq     = make_row(4, "Normal Queue:",    str(data.get("normal_queue_size", 0)))
        lbl_eq     = make_row(5, "Emergency Queue:", str(data.get("emergency_queue_size", 0)))

        ctk.CTkFrame(card, height=6, fg_color="transparent").grid(row=6, column=0)

        card.lbls = {
            "badge":  badge_lbl,
            "sensor": lbl_sensor,
            "wait":   lbl_wait,
            "nq":     lbl_nq,
            "eq":     lbl_eq,
        }
        return card

    def _build_action_row(self):
        action_frame = ctk.CTkFrame(self, fg_color="transparent")
        action_frame.grid(row=2, column=0, sticky="ew", padx=16, pady=(4, 16))

        ctk.CTkButton(
            action_frame, text="➕  Add Normal Vehicle",
            fg_color=GREEN, hover_color="#237a35",
            corner_radius=8, font=ctk.CTkFont(size=13, weight="bold"),
            height=38, command=lambda: self.app.show_page("normal_vehicles")
        ).pack(side="left", padx=(0, 8))

        ctk.CTkButton(
            action_frame, text="🚨  Add Emergency Vehicle",
            fg_color=RED, hover_color="#a82a28",
            corner_radius=8, font=ctk.CTkFont(size=13, weight="bold"),
            height=38, command=lambda: self.app.show_page("emergency_vehicles")
        ).pack(side="left", padx=(0, 8))

        ctk.CTkButton(
            action_frame, text="🔄  Refresh",
            fg_color=ACCENT, hover_color="#1558c0",
            corner_radius=8, font=ctk.CTkFont(size=13, weight="bold"),
            height=38, command=self.refresh_data
        ).pack(side="left")

    def refresh_data(self):
        if not self.app.backend.connected:
            return

        roads_res = self.app.backend.send_request({"operation": "get_roads"})
        if not roads_res or not roads_res.get("success"):
            self.app.log_activity("Failed to fetch road list", False)
            return

        roads_data = roads_res["data"].get("roads", [])

        for widget in self.scroll_area.winfo_children():
            widget.destroy()
        self.road_card_frames.clear()

        total_vehicles = 0
        total_normal   = 0
        total_emerg    = 0

        for idx, road_info in enumerate(roads_data):
            road_id      = road_info.get("road_id", "")
            display_name = road_info.get("display_name", road_id)

            card = self._build_road_card(
                self.scroll_area, road_id, display_name, road_info, idx
            )
            self.road_card_frames[road_id] = card

            nq = road_info.get("normal_queue_size", 0)
            eq = road_info.get("emergency_queue_size", 0)
            total_normal += nq
            total_emerg  += eq
            total_vehicles += nq + eq

        self.total_vehicles_lbl.configure(text=str(total_vehicles))
        self.normal_queue_lbl.configure(text=str(total_normal))
        self.emerg_queue_lbl.configure(text=str(total_emerg))
        self.active_roads_lbl.configure(text=str(len(roads_data)))

        self.app.log_activity(f"Dashboard refreshed — {len(roads_data)} road(s) loaded", True)
