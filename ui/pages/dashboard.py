import customtkinter as ctk

class DashboardPage(ctk.CTkFrame):
    def __init__(self, master, app):
        super().__init__(master, fg_color="transparent")
        self.app = app
        
        self.grid_columnconfigure((0, 1), weight=1)
        self.grid_rowconfigure(1, weight=1)

        # Summary Frame
        self.summary_frame = ctk.CTkFrame(self)
        self.summary_frame.grid(row=0, column=0, columnspan=2, sticky="ew", pady=(0, 20))
        self.summary_frame.grid_columnconfigure((0, 1, 2, 3), weight=1)

        self.normal_lbl = self.create_summary_card(self.summary_frame, "Normal Waiting", "0", 0)
        self.emerg_lbl = self.create_summary_card(self.summary_frame, "Emergency Waiting", "0", 1)
        self.north_sensor_lbl = self.create_summary_card(self.summary_frame, "North Sensor", "0", 2)
        self.south_sensor_lbl = self.create_summary_card(self.summary_frame, "South Sensor", "0", 3)

        # Road Overviews
        self.north_frame = self.create_road_card("North Road", 0)
        self.south_frame = self.create_road_card("South Road", 1)

        # Middle Action Buttons (Large Add Buttons)
        self.middle_action_frame = ctk.CTkFrame(self, fg_color="transparent")
        self.middle_action_frame.grid(row=2, column=0, columnspan=2, pady=30)
        
        btn_font = ctk.CTkFont(size=16, weight="bold")
        
        btn_add_n = ctk.CTkButton(self.middle_action_frame, text="Add Normal Vehicle", font=btn_font, width=220, height=45,
                                  command=lambda: self.app.show_page("normal_vehicles"))
        btn_add_n.pack(side="left", padx=20)
        
        btn_add_e = ctk.CTkButton(self.middle_action_frame, text="Add Emergency Vehicle", font=btn_font, width=220, height=45,
                                  fg_color="#C0392B", hover_color="#922B21",
                                  command=lambda: self.app.show_page("emergency_vehicles"))
        btn_add_e.pack(side="left", padx=20)

        # Bottom Action Buttons
        self.bottom_action_frame = ctk.CTkFrame(self, fg_color="transparent")
        self.bottom_action_frame.grid(row=3, column=0, columnspan=2, sticky="ew", pady=(0, 20))
        self.bottom_action_frame.grid_columnconfigure(1, weight=1)
        
        btn_refresh = ctk.CTkButton(self.bottom_action_frame, text="Refresh Dashboard", command=self.refresh_data)
        btn_refresh.grid(row=0, column=0, padx=10, sticky="w")
        
        btn_update_sensor = ctk.CTkButton(self.bottom_action_frame, text="Update Road Sensor", 
                                          command=lambda: self.app.show_page("road_sensors"))
        btn_update_sensor.grid(row=0, column=2, padx=10, sticky="e")

    def create_summary_card(self, parent, title, initial_val, col):
        frame = ctk.CTkFrame(parent, corner_radius=10)
        frame.grid(row=0, column=col, padx=10, pady=10, sticky="nsew")
        ctk.CTkLabel(frame, text=title, text_color="gray").pack(pady=(10, 0))
        val_lbl = ctk.CTkLabel(frame, text=initial_val, font=ctk.CTkFont(size=28, weight="bold"))
        val_lbl.pack(pady=(0, 10))
        return val_lbl

    def create_road_card(self, title, col):
        frame = ctk.CTkFrame(self, corner_radius=10)
        frame.grid(row=1, column=col, padx=10, sticky="nsew")
        ctk.CTkLabel(frame, text=title, font=ctk.CTkFont(size=18, weight="bold")).pack(pady=10)
        
        lbl_sensor = ctk.CTkLabel(frame, text="Simulated Sensor Count: -")
        lbl_sensor.pack(anchor="w", padx=20, pady=5)
        
        lbl_wait = ctk.CTkLabel(frame, text="Simulated Max Wait Time: -")
        lbl_wait.pack(anchor="w", padx=20, pady=5)
        
        lbl_nq = ctk.CTkLabel(frame, text="Normal Queue Size: -")
        lbl_nq.pack(anchor="w", padx=20, pady=5)
        
        lbl_eq = ctk.CTkLabel(frame, text="Emergency Queue Size: -")
        lbl_eq.pack(anchor="w", padx=20, pady=5)
        
        frame.lbls = {"sensor": lbl_sensor, "wait": lbl_wait, "nq": lbl_nq, "eq": lbl_eq}
        return frame

    def refresh_data(self):
        if not self.app.backend.connected:
            return

        north_res = self.app.backend.send_request({"operation": "get_traffic_info", "road_id": "NORTH"})
        south_res = self.app.backend.send_request({"operation": "get_traffic_info", "road_id": "SOUTH"})

        if north_res and north_res.get("success") and south_res and south_res.get("success"):
            ndata = north_res["data"]
            sdata = south_res["data"]

            self.normal_lbl.configure(text=str(ndata["normal_queue_size"] + sdata["normal_queue_size"]))
            self.emerg_lbl.configure(text=str(ndata["emergency_queue_size"] + sdata["emergency_queue_size"]))
            self.north_sensor_lbl.configure(text=str(ndata["sensor_vehicle_count"]))
            self.south_sensor_lbl.configure(text=str(sdata["sensor_vehicle_count"]))

            self.update_road_card(self.north_frame, ndata)
            self.update_road_card(self.south_frame, sdata)
            self.app.log_activity("Dashboard refreshed", True)
        else:
            self.app.log_activity("Failed to refresh dashboard", False)

    def update_road_card(self, frame, data):
        frame.lbls["sensor"].configure(text=f"Simulated Sensor Count: {data['sensor_vehicle_count']}")
        frame.lbls["wait"].configure(text=f"Simulated Max Wait Time: {data['max_waiting_time']}s")
        frame.lbls["nq"].configure(text=f"Normal Queue Size: {data['normal_queue_size']}")
        frame.lbls["eq"].configure(text=f"Emergency Queue Size: {data['emergency_queue_size']}")
