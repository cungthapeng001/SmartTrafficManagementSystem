import customtkinter as ctk

class SettingsPage(ctk.CTkFrame):
    def __init__(self, master, app):
        super().__init__(master, fg_color="transparent")
        self.app = app
        
        self.grid_columnconfigure(0, weight=1)
        
        # Info Frame
        self.info_frame = ctk.CTkFrame(self, corner_radius=10)
        self.info_frame.grid(row=0, column=0, sticky="nsew", pady=10)
        
        info_text = (
            "Project: Smart Traffic Management System\n\n"
            "Phase: Phase 2\n"
            "Frontend: Python 3 + CustomTkinter\n"
            "Backend: C++17\n"
            "Data Structures: std::queue, std::priority_queue\n"
        )
        
        ctk.CTkLabel(self.info_frame, text="System Information", font=ctk.CTkFont(size=18, weight="bold")).pack(pady=20)
        ctk.CTkLabel(self.info_frame, text=info_text, justify="left").pack(padx=20, pady=10)
        
        # Status Frame
        self.status_frame = ctk.CTkFrame(self, corner_radius=10)
        self.status_frame.grid(row=1, column=0, sticky="nsew", pady=10)
        
        ctk.CTkLabel(self.status_frame, text="Backend Control", font=ctk.CTkFont(size=18, weight="bold")).pack(pady=20)
        
        self.status_lbl = ctk.CTkLabel(self.status_frame, text="Current Status: Unknown")
        self.status_lbl.pack(pady=10)
        
        ctk.CTkButton(self.status_frame, text="Reconnect Backend", command=self.reconnect_backend).pack(pady=20)

    def refresh_data(self):
        if self.app.backend and self.app.backend.connected:
            self.status_lbl.configure(text="Current Status: Connected and Running", text_color="#2ECC71")
        else:
            self.status_lbl.configure(text="Current Status: Disconnected", text_color="#E74C3C")

    def reconnect_backend(self):
        if self.app.backend:
            self.app.backend.stop()
        self.app.init_backend()
        self.app.update_connection_status()
        self.refresh_data()
