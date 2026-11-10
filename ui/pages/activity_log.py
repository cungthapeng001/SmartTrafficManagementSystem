import customtkinter as ctk

BG_DARK    = "#0D1117"
BG_CARD    = "#161B22"
ACCENT     = "#1F6FEB"
TEXT_MAIN  = "#E6EDF3"
TEXT_MUTED = "#8B949E"
GREEN      = "#2EA043"
RED        = "#DA3633"
BORDER     = "#30363D"

class ActivityLogPage(ctk.CTkFrame):

    def __init__(self, master, app):
        super().__init__(master, fg_color="transparent")
        self.app = app

        self.grid_columnconfigure(0, weight=1)
        self.grid_rowconfigure(1, weight=1)

        self._build_top_bar()

        self._build_log_area()

        self._build_action_bar()

    def _build_top_bar(self):
        top_bar = ctk.CTkFrame(self, fg_color="transparent")
        top_bar.grid(row=0, column=0, sticky="ew", padx=16, pady=(16, 8))
        top_bar.grid_columnconfigure(0, weight=1)

        ctk.CTkLabel(
            top_bar,
            text="Activity Log",
            font=ctk.CTkFont(size=16, weight="bold"),
            text_color=TEXT_MAIN
        ).grid(row=0, column=0, sticky="w")

        self.count_lbl = ctk.CTkLabel(
            top_bar,
            text="0 entries",
            font=ctk.CTkFont(size=12),
            text_color=TEXT_MUTED
        )
        self.count_lbl.grid(row=0, column=1, sticky="e")

    def _build_log_area(self):
        log_card = ctk.CTkFrame(
            self, fg_color=BG_CARD, corner_radius=12,
            border_width=1, border_color=BORDER
        )
        log_card.grid(row=1, column=0, sticky="nsew", padx=16, pady=(0, 8))
        log_card.grid_columnconfigure(0, weight=1)
        log_card.grid_rowconfigure(0, weight=1)

        self.log_frame = ctk.CTkScrollableFrame(
            log_card,
            fg_color="transparent",
            scrollbar_button_color=BORDER,
            scrollbar_button_hover_color=ACCENT
        )
        self.log_frame.grid(row=0, column=0, sticky="nsew", padx=4, pady=4)
        self.log_frame.grid_columnconfigure(0, weight=1)

        self.log_widgets = []

    def _build_action_bar(self):
        self.action_frame = ctk.CTkFrame(self, fg_color="transparent")
        self.action_frame.grid(row=2, column=0, sticky="ew",
                               padx=16, pady=(0, 16))

        ctk.CTkLabel(
            self.action_frame,
            text="Logs are stored in memory and cleared on exit.",
            font=ctk.CTkFont(size=11),
            text_color=TEXT_MUTED
        ).pack(side="left")

        ctk.CTkButton(
            self.action_frame,
            text="🗑  Clear Log",
            fg_color=BG_CARD,
            hover_color=BORDER,
            border_color=BORDER,
            border_width=1,
            text_color=TEXT_MUTED,
            corner_radius=8,
            font=ctk.CTkFont(size=12),
            height=34,
            command=self.clear_logs
        ).pack(side="right")

    def clear_logs(self):
        self.app.activity_logs.clear()
        self.refresh_data()

    def refresh_data(self):
        for w in self.log_widgets:
            w.destroy()
        self.log_widgets.clear()

        entries = self.app.activity_logs

        self.count_lbl.configure(text=f"{len(entries)} {'entry' if len(entries) == 1 else 'entries'}")

        for idx, log in enumerate(entries):
            is_success = log["status"] == "SUCCESS"
            border_color = GREEN if is_success else RED
            status_text  = log["status"]

            entry_frame = ctk.CTkFrame(
                self.log_frame,
                fg_color=BG_DARK,
                corner_radius=8,
                border_width=1,
                border_color=BORDER
            )
            entry_frame.grid(row=idx, column=0, sticky="ew",
                             padx=8, pady=3)
            entry_frame.grid_columnconfigure(2, weight=1)

            accent_strip = ctk.CTkFrame(
                entry_frame,
                width=4,
                fg_color=border_color,
                corner_radius=4
            )
            accent_strip.grid(row=0, column=0, padx=(6, 0), pady=6, sticky="ns")

            status_badge = ctk.CTkLabel(
                entry_frame,
                text=status_text,
                font=ctk.CTkFont(size=10, weight="bold"),
                text_color=BG_DARK,
                fg_color=border_color,
                corner_radius=4,
                width=65,
                height=22
            )
            status_badge.grid(row=0, column=1, padx=(8, 6), pady=8)

            msg_text = f"[{log['time']}]  {log['message']}"
            msg_lbl = ctk.CTkLabel(
                entry_frame,
                text=msg_text,
                font=ctk.CTkFont(size=12),
                text_color=TEXT_MAIN if is_success else "#F0A0A0",
                anchor="w",
                justify="left",
                wraplength=700
            )
            msg_lbl.grid(row=0, column=2, padx=(0, 12), pady=8, sticky="w")

            self.log_widgets.append(entry_frame)
