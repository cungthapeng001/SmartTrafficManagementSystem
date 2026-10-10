import customtkinter as ctk

class ActivityLogPage(ctk.CTkFrame):
    def __init__(self, master, app):
        super().__init__(master, fg_color="transparent")
        self.app = app
        
        self.grid_columnconfigure(0, weight=1)
        self.grid_rowconfigure(0, weight=1)

        self.log_frame = ctk.CTkScrollableFrame(self)
        self.log_frame.grid(row=0, column=0, sticky="nsew", pady=(0, 20))
        self.log_widgets = []

        self.action_frame = ctk.CTkFrame(self, fg_color="transparent")
        self.action_frame.grid(row=1, column=0, sticky="ew")
        
        ctk.CTkButton(self.action_frame, text="Clear Log", fg_color="gray", command=self.clear_logs).pack(side="right")
        ctk.CTkLabel(self.action_frame, text="Logs are stored in memory and cleared on exit.", text_color="gray").pack(side="left")

    def clear_logs(self):
        self.app.activity_logs.clear()
        self.refresh_data()

    def refresh_data(self):
        for w in self.log_widgets:
            w.destroy()
        self.log_widgets.clear()
        
        for idx, log in enumerate(self.app.activity_logs):
            color = "#2ECC71" if log["status"] == "SUCCESS" else "#E74C3C"
            lbl = ctk.CTkLabel(self.log_frame, text=f"[{log['time']}] {log['status']}: {log['message']}", text_color=color, anchor="w")
            lbl.grid(row=idx, column=0, sticky="w", padx=10, pady=2)
            self.log_widgets.append(lbl)
