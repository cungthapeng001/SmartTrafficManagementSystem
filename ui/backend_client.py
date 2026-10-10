import subprocess
import json
import threading
import os

class BackendClient:
    def __init__(self, executable_path):
        self.executable_path = executable_path
        self.process = None
        self.lock = threading.Lock()
        self.connected = False
        
    def start(self):
        if not os.path.exists(self.executable_path):
            raise FileNotFoundError(f"Backend executable not found at {self.executable_path}")
            
        self.process = subprocess.Popen(
            [self.executable_path, "--backend"],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1
        )
        
        # Verify connection with a ping
        res = self.send_request({"operation": "ping"})
        if res and res.get("success"):
            self.connected = True
        else:
            self.connected = False
            raise Exception("Failed to ping backend after starting.")
            
    def stop(self):
        if self.process:
            self.send_request({"operation": "shutdown"})
            self.process.terminate()
            self.process.wait()
            self.process = None
            self.connected = False

    def send_request(self, req_dict):
        with self.lock:
            if not self.process or self.process.poll() is not None:
                self.connected = False
                return {"success": False, "message": "Backend process is not running.", "data": None}
            
            try:
                # Send JSON string
                req_str = json.dumps(req_dict)
                self.process.stdin.write(req_str + "\n")
                self.process.stdin.flush()
                
                # Read response
                response_str = self.process.stdout.readline()
                if not response_str:
                    self.connected = False
                    return {"success": False, "message": "Backend closed connection.", "data": None}
                    
                return json.loads(response_str)
            except Exception as e:
                self.connected = False
                return {"success": False, "message": f"Communication error: {str(e)}", "data": None}
