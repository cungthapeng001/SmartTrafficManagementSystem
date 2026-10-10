import customtkinter as ctk

class RoadSensorsPage(ctk.CTkFrame):
    def __init__(self, master, app):
        super().__init__(master, fg_color="transparent")
        self.app = app
        
        self.grid_columnconfigure((0, 1), weight=1)
        self.grid_rowconfigure(0, weight=1)

        # Update Form
        self.form_frame = ctk.CTkFrame(self, corner_radius=10)
        self.form_frame.grid(row=0, column=0, padx=10, pady=10, sticky="nsew")
        
        ctk.CTkLabel(self.form_frame, text="Update Simulated Sensors", font=ctk.CTkFont(size=18, weight="bold")).pack(pady=20)
        
        self.road_var = ctk.StringVar(value="NORTH")
        ctk.CTkLabel(self.form_frame, text="Road Selection:").pack(pady=(10, 5))
        ctk.CTkOptionMenu(self.form_frame, variable=self.road_var, values=["NORTH", "SOUTH"]).pack()

        self.count_var = ctk.StringVar(value="0")
        ctk.CTkLabel(self.form_frame, text="Simulated Vehicle Count:").pack(pady=(10, 5))
        ctk.CTkEntry(self.form_frame, textvariable=self.count_var).pack()

        self.wait_var = ctk.StringVar(value="0")
        ctk.CTkLabel(self.form_frame, text="Simulated Max Wait Time:").pack(pady=(10, 5))
        ctk.CTkEntry(self.form_frame, textvariable=self.wait_var).pack()

        btn = ctk.CTkButton(self.form_frame, text="Update Sensors", command=self.update_sensors)
        btn.pack(pady=30)

        # Current Status
        self.status_frame = ctk.CTkFrame(self, corner_radius=10)
        self.status_frame.grid(row=0, column=1, padx=10, pady=10, sticky="nsew")
        
        ctk.CTkLabel(self.status_frame, text="Current Sensor Status", font=ctk.CTkFont(size=18, weight="bold")).pack(pady=20)
        
        self.north_status = ctk.CTkLabel(self.status_frame, text="North Road:\nCount: -\nMax Wait: -")
        self.north_status.pack(pady=20)
        
        self.south_status = ctk.CTkLabel(self.status_frame, text="South Road:\nCount: -\nMax Wait: -")
        self.south_status.pack(pady=20)
        
        ctk.CTkButton(self.status_frame, text="Refresh Info", command=self.refresh_data).pack(pady=30)

    def update_sensors(self):
        if not self.app.backend.connected:
            return
            
        try:
            count = int(self.count_var.get())
            wait = int(self.wait_var.get())
            if count < 0 or wait < 0: raise ValueError
        except:
            self.app.log_activity("Invalid sensor values (must be non-negative integers).", False)
            return

        req = {
            "operation": "update_sensor_info",
            "road_id": self.road_var.get(),
            "sensor_count": count,
            "max_wait_time": wait
        }
        res = self.app.backend.send_request(req)
        self.app.log_activity(res.get("message", "Error"), res.get("success", False))
        if res.get("success"):
            self.refresh_data()

    def refresh_data(self):
        if not self.app.backend.connected:
            return
            
        n_res = self.app.backend.send_request({"operation": "get_traffic_info", "road_id": "NORTH"})
        if n_res and n_res.get("success"):
            ndata = n_res["data"]
            self.north_status.configure(text=f"North Road:\nCount: {ndata['sensor_vehicle_count']}\nMax Wait: {ndata['max_waiting_time']}s")
            
        s_res = self.app.backend.send_request({"operation": "get_traffic_info", "road_id": "SOUTH"})
        if s_res and s_res.get("success"):
            sdata = s_res["data"]
            self.south_status.configure(text=f"South Road:\nCount: {sdata['sensor_vehicle_count']}\nMax Wait: {sdata['max_waiting_time']}s")
