COLORS = {
    "bg_dark":    "#0D1117",
    "bg_card":    "#161B22",
    "accent":     "#1F6FEB",
    "text_main":  "#E6EDF3",
    "text_muted": "#8B949E",
    "green":      "#2EA043",
    "yellow":     "#D29922",
    "red":        "#DA3633",
    "border":     "#30363D",
    "bg_row_alt": "#1C2128",
}

TRAFFIC_LOW_MAX_COUNT = 5
TRAFFIC_LOW_MAX_WAIT  = 30
TRAFFIC_MED_MAX_COUNT = 15
TRAFFIC_MED_MAX_WAIT  = 60

GREEN_LOW       = 20
GREEN_MEDIUM    = 35
GREEN_HIGH      = 50
GREEN_EMERGENCY = 60

def get_traffic_level(vehicle_count, max_wait):
    if vehicle_count <= TRAFFIC_LOW_MAX_COUNT and max_wait <= TRAFFIC_LOW_MAX_WAIT:
        return ("LOW", COLORS["green"])
    elif vehicle_count <= TRAFFIC_MED_MAX_COUNT and max_wait <= TRAFFIC_MED_MAX_WAIT:
        return ("MEDIUM", COLORS["yellow"])
    else:
        return ("HIGH", COLORS["red"])

def get_green_duration(level_str):
    if level_str == "LOW":
        return GREEN_LOW
    elif level_str == "MEDIUM":
        return GREEN_MEDIUM
    else:
        return GREEN_HIGH
