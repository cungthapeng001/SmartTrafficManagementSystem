import customtkinter as ctk

# ── Color palette ──────────────────────────────────────────────────────────────
BG_DARK    = "#0D1117"
BG_CARD    = "#161B22"
ACCENT     = "#1F6FEB"
TEXT_MAIN  = "#E6EDF3"
TEXT_MUTED = "#8B949E"
GREEN      = "#2EA043"
RED        = "#DA3633"
BORDER     = "#30363D"


class SettingsPage(ctk.CTkFrame):
    """
    Settings & About page.
    Shows system information card and backend control card.
    Backend status updates on page load; reconnect button re-initialises
    the C++ backend subprocess.
    """

    def __init__(self, master, app):
        super().__init__(master, fg_color="transparent")
        self.app = app

        self.grid_columnconfigure(0, weight=1)
        self.grid_rowconfigure(2, weight=1)   # Spacer row at bottom

        # ── System info card ───────────────────────────────────────────────
        self._build_info_card()

        # ── Backend status / control card ──────────────────────────────────
        self._build_status_card()

    # ── System info card ────────────────────────────────────────────────────────

    def _build_info_card(self):
        """Card showing static project/stack information."""
        self.info_frame = ctk.CTkFrame(
            self, fg_color=BG_CARD, corner_radius=12,
            border_width=1, border_color=BORDER
        )
        self.info_frame.grid(row=0, column=0, sticky="ew",
                             padx=16, pady=(16, 8))
        self.info_frame.grid_columnconfigure(0, weight=1)

        ctk.CTkLabel(
            self.info_frame,
            text="⚙️  System Information",
            font=ctk.CTkFont(size=16, weight="bold"),
            text_color=TEXT_MAIN
        ).grid(row=0, column=0, sticky="w", padx=16, pady=(20, 8))

        ctk.CTkFrame(self.info_frame, height=1, fg_color=BORDER).grid(
            row=1, column=0, padx=16, pady=(0, 12), sticky="ew"
        )

        # System info rows as (label, value) pairs
        info_rows = [
            ("Project",           "Smart Traffic Management System"),
            ("Architecture",      "Multi-Road Traffic Architecture"),
            ("Frontend",          "Python 3 + CustomTkinter"),
            ("Backend",           "C++17"),
            ("Data Structures",   "std::vector<Road>, std::queue, std::priority_queue"),
            ("Graph Algorithm",   "Dijkstra's Algorithm (C++ Graph engine)"),
        ]

        for i, (label_text, value_text) in enumerate(info_rows):
            row_frame = ctk.CTkFrame(self.info_frame, fg_color="transparent")
            row_frame.grid(row=i + 2, column=0, padx=16, pady=5, sticky="ew")
            row_frame.grid_columnconfigure(1, weight=1)

            # Field label (muted)
            ctk.CTkLabel(
                row_frame,
                text=label_text + ":",
                font=ctk.CTkFont(size=13),
                text_color=TEXT_MUTED,
                width=160,
                anchor="w"
            ).grid(row=0, column=0, sticky="w")

            # Field value (main)
            ctk.CTkLabel(
                row_frame,
                text=value_text,
                font=ctk.CTkFont(size=13, weight="bold"),
                text_color=TEXT_MAIN,
                anchor="w"
            ).grid(row=0, column=1, sticky="w")

        # Spacer at bottom of card
        ctk.CTkFrame(self.info_frame, height=12, fg_color="transparent").grid(
            row=len(info_rows) + 2, column=0
        )

    # ── Backend status card ─────────────────────────────────────────────────────

    def _build_status_card(self):
        """Card showing live backend connection state and a reconnect button."""
        self.status_frame = ctk.CTkFrame(
            self, fg_color=BG_CARD, corner_radius=12,
            border_width=1, border_color=BORDER
        )
        self.status_frame.grid(row=1, column=0, sticky="ew",
                               padx=16, pady=(0, 16))
        self.status_frame.grid_columnconfigure(0, weight=1)

        ctk.CTkLabel(
            self.status_frame,
            text="🔌  Backend Control",
            font=ctk.CTkFont(size=16, weight="bold"),
            text_color=TEXT_MAIN
        ).grid(row=0, column=0, sticky="w", padx=16, pady=(20, 8))

        ctk.CTkFrame(self.status_frame, height=1, fg_color=BORDER).grid(
            row=1, column=0, padx=16, pady=(0, 12), sticky="ew"
        )

        # Status indicator row: colored dot + text
        status_row = ctk.CTkFrame(self.status_frame, fg_color="transparent")
        status_row.grid(row=2, column=0, padx=16, pady=8, sticky="w")

        ctk.CTkLabel(
            status_row,
            text="Connection Status:",
            font=ctk.CTkFont(size=13),
            text_color=TEXT_MUTED
        ).grid(row=0, column=0, sticky="w", padx=(0, 12))

        # This label shows the colored status indicator text
        self.status_lbl = ctk.CTkLabel(
            status_row,
            text="● Unknown",
            font=ctk.CTkFont(size=13, weight="bold"),
            text_color=TEXT_MUTED
        )
        self.status_lbl.grid(row=0, column=1, sticky="w")

        # Reconnect button
        ctk.CTkButton(
            self.status_frame,
            text="🔄  Reconnect Backend",
            fg_color=ACCENT,
            hover_color="#1558c0",
            corner_radius=8,
            font=ctk.CTkFont(size=13, weight="bold"),
            height=40,
            command=self.reconnect_backend
        ).grid(row=3, column=0, padx=16, pady=(8, 24), sticky="w")

    # ── Backend operations (unchanged logic) ───────────────────────────────────

    def refresh_data(self):
        """
        Update the backend connection status indicator.
        Called automatically when the user navigates to this page.
        """
        if self.app.backend and self.app.backend.connected:
            self.status_lbl.configure(
                text="● Connected and Running",
                text_color=GREEN
            )
        else:
            self.status_lbl.configure(
                text="● Disconnected",
                text_color=RED
            )

    def reconnect_backend(self):
        """
        Stop the existing backend process (if any), re-initialise it,
        and update all connection indicators.
        """
        if self.app.backend:
            self.app.backend.stop()
        self.app.init_backend()
        self.app.update_connection_status()
        self.refresh_data()
