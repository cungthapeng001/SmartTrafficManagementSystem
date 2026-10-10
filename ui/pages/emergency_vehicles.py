import customtkinter as ctk

class EmergencyVehiclesPage(ctk.CTkFrame):
    def __init__(self, master, app):
        super().__init__(master, fg_color="transparent")
        self.app = app
        
        self.grid_columnconfigure(0, weight=1)
        self.grid_rowconfigure(1, weight=1)

        # Form Frame
        self.form_frame = ctk.CTkFrame(self)
        self.form_frame.grid(row=0, column=0, sticky="ew", pady=(0, 20))
        self.form_frame.grid_columnconfigure((0, 1, 2, 3), weight=1)
        
        self.id_var = ctk.StringVar()
        self.type_var = ctk.StringVar(value="Ambulance")
        self.road_var = ctk.StringVar(value="NORTH")
        self.wait_var = ctk.StringVar(value="0")

        ctk.CTkLabel(self.form_frame, text="Vehicle ID:").grid(row=0, column=0, padx=10, pady=10)
        ctk.CTkEntry(self.form_frame, textvariable=self.id_var).grid(row=0, column=1, padx=10, pady=10)

        ctk.CTkLabel(self.form_frame, text="Emergency Type:").grid(row=0, column=2, padx=10, pady=10)
        ctk.CTkOptionMenu(self.form_frame, variable=self.type_var, values=["Ambulance", "Fire Truck", "Police Vehicle"]).grid(row=0, column=3, padx=10, pady=10)

        ctk.CTkLabel(self.form_frame, text="Road:").grid(row=1, column=0, padx=10, pady=10)
        ctk.CTkOptionMenu(self.form_frame, variable=self.road_var, values=["NORTH", "SOUTH"]).grid(row=1, column=1, padx=10, pady=10)

        ctk.CTkLabel(self.form_frame, text="Wait Time (s):").grid(row=1, column=2, padx=10, pady=10)
        ctk.CTkEntry(self.form_frame, textvariable=self.wait_var).grid(row=1, column=3, padx=10, pady=10)

        # Center the button below the inputs
        btn = ctk.CTkButton(self.form_frame, text="Add Emergency Vehicle", 
                            font=ctk.CTkFont(size=16, weight="bold"),
                            width=220, height=45,
                            fg_color="#C0392B", hover_color="#922B21", 
                            command=self.add_vehicle)
        btn.grid(row=2, column=0, columnspan=4, pady=(15, 20))

        # Table Frame
        self.table_frame = ctk.CTkScrollableFrame(self)
        self.table_frame.grid(row=1, column=0, sticky="nsew")
        self.table_frame.grid_columnconfigure((0, 1, 2, 3, 4), weight=1)
        
        self.setup_table_headers()
        self.row_widgets = []

        # Actions
        self.action_frame = ctk.CTkFrame(self, fg_color="transparent")
        self.action_frame.grid(row=2, column=0, sticky="ew", pady=20)
        
        ctk.CTkButton(self.action_frame, text="Process North Vehicle", command=lambda: self.process_vehicle("NORTH")).pack(side="left", padx=10)
        ctk.CTkButton(self.action_frame, text="Process South Vehicle", command=lambda: self.process_vehicle("SOUTH")).pack(side="left", padx=10)
        ctk.CTkButton(self.action_frame, text="Refresh Queue", command=self.refresh_data).pack(side="right", padx=10)

    def setup_table_headers(self):
        headers = ["Priority", "Vehicle ID", "Emergency Type", "Wait Time", "Road"]
        for i, h in enumerate(headers):
            lbl = ctk.CTkLabel(self.table_frame, text=h, font=ctk.CTkFont(weight="bold"))
            lbl.grid(row=0, column=i, pady=5, padx=5, sticky="w")

    def clear_table(self):
        for widget in self.row_widgets:
            widget.destroy()
        self.row_widgets.clear()

    def add_row(self, row_idx, priority, vid, vtype, wait, road):
        w1 = ctk.CTkLabel(self.table_frame, text=str(priority), text_color="#E74C3C")
        w1.grid(row=row_idx, column=0, pady=2, padx=5, sticky="w")
        w2 = ctk.CTkLabel(self.table_frame, text=vid)
        w2.grid(row=row_idx, column=1, pady=2, padx=5, sticky="w")
        w3 = ctk.CTkLabel(self.table_frame, text=vtype)
        w3.grid(row=row_idx, column=2, pady=2, padx=5, sticky="w")
        w4 = ctk.CTkLabel(self.table_frame, text=str(wait))
        w4.grid(row=row_idx, column=3, pady=2, padx=5, sticky="w")
        w5 = ctk.CTkLabel(self.table_frame, text=road)
        w5.grid(row=row_idx, column=4, pady=2, padx=5, sticky="w")
        self.row_widgets.extend([w1, w2, w3, w4, w5])

    def add_vehicle(self):
        if not self.app.backend.connected:
            return
        
        try:
            wait = int(self.wait_var.get())
            if wait < 0: raise ValueError
        except:
            self.app.log_activity("Invalid waiting time", False)
            return

        type_val = self.type_var.get()
        priority = 3
        if type_val == "Ambulance": priority = 1
        elif type_val == "Fire Truck": priority = 2

        req = {
            "operation": "add_emergency_vehicle",
            "road_id": self.road_var.get(),
            "vehicle_id": self.id_var.get(),
            "vehicle_type": type_val,
            "waiting_time": wait,
            "priority": priority
        }
        res = self.app.backend.send_request(req)
        self.app.log_activity(res.get("message", "Error"), res.get("success", False))
        if res.get("success"):
            self.id_var.set("") # Clear ID
            self.refresh_data()

    def process_vehicle(self, road):
        if not self.app.backend.connected:
            return
        res = self.app.backend.send_request({"operation": "process_emergency_vehicle", "road_id": road})
        self.app.log_activity(res.get("message", "Error"), res.get("success", False))
        if res.get("success"):
            self.refresh_data()

    def refresh_data(self):
        if not self.app.backend.connected:
            return
        self.clear_table()
        
        row_idx = 1
        for road in ["NORTH", "SOUTH"]:
            res = self.app.backend.send_request({"operation": "get_emergency_vehicles", "road_id": road})
            if res and res.get("success"):
                for v in res["data"]["vehicles"]:
                    self.add_row(row_idx, v.get("priority", "-"), v["vehicle_id"], v["vehicle_type"], v["wait_time"], road)
                    row_idx += 1
