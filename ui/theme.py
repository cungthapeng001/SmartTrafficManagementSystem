# =============================================================
# theme.py — Shared design constants for the STMS UI
#
# Import this in any page with:
#   from theme import COLORS, font
# =============================================================

# ---- Color palette ------------------------------------------
COLORS = {
    "bg_dark":    "#0D1117",   # Main window / sidebar background
    "bg_card":    "#161B22",   # Card and panel background
    "accent":     "#1F6FEB",   # Blue accent (buttons, highlights)
    "text_main":  "#E6EDF3",   # Primary text
    "text_muted": "#8B949E",   # Secondary / muted text
    "green":      "#2EA043",   # Success / low traffic
    "yellow":     "#D29922",   # Warning / medium traffic
    "red":        "#DA3633",   # Error / high traffic / emergency
    "border":     "#30363D",   # Card borders / dividers
    "bg_row_alt": "#1C2128",   # Alternate table row color
}

# ---- Traffic classification thresholds ----------------------
# These match the C++ TrafficSignal and Graph::updateTrafficOnRoad
# thresholds exactly, so the UI preview stays consistent.
TRAFFIC_LOW_MAX_COUNT = 5
TRAFFIC_LOW_MAX_WAIT  = 30
TRAFFIC_MED_MAX_COUNT = 15
TRAFFIC_MED_MAX_WAIT  = 60

# ---- Green-light durations (seconds) ------------------------
# Must match TrafficSignal.h constants in the C++ backend.
GREEN_LOW       = 20
GREEN_MEDIUM    = 35
GREEN_HIGH      = 50
GREEN_EMERGENCY = 60


def get_traffic_level(vehicle_count, max_wait):
    """
    Classify traffic into LOW / MEDIUM / HIGH.
    Uses the same rules as the C++ backend:
      LOW    : count ≤ 5  AND wait ≤ 30s
      MEDIUM : count ≤ 15 AND wait ≤ 60s
      HIGH   : otherwise
    Returns a tuple (level_str, color_hex).
    """
    if vehicle_count <= TRAFFIC_LOW_MAX_COUNT and max_wait <= TRAFFIC_LOW_MAX_WAIT:
        return ("LOW", COLORS["green"])
    elif vehicle_count <= TRAFFIC_MED_MAX_COUNT and max_wait <= TRAFFIC_MED_MAX_WAIT:
        return ("MEDIUM", COLORS["yellow"])
    else:
        return ("HIGH", COLORS["red"])


def get_green_duration(level_str):
    """
    Return proposed green-light duration in seconds for a traffic level.
    Matches the GREEN_* constants in TrafficSignal.h.
    """
    if level_str == "LOW":
        return GREEN_LOW
    elif level_str == "MEDIUM":
        return GREEN_MEDIUM
    else:
        return GREEN_HIGH

