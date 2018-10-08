"resource/ui/leaderboards_filter.res"
{

    "InputLabel"
    {
        "ControlName" "Label"
        "fieldName" "InputLabel"
        "xpos" "2"
        "ypos" "2"
        "labelText" "#MOM_RunFlag_Input"
        "auto_wide_tocontents" "1"
    }

    "ScrollOnly"
    {
        "ControlName" "ToggleButton"
        "fieldName" "ScrollOnly"
        "xpos" "5"
        "ypos" "15"
        "wide"		"88"
		"tall"		"16"
        "labelText" "#MOM_RunFlag_ScrollOnly"
        "auto_wide_tocontents" "1"
        "textAlignment"		"center"
        "command" "ToggleScrollOnly"
        "actionsignallevel" "2" // Needed so that the leaderboards class can handle it
        "Font"			"BigNoodleSmall"
    }
    
    // Auto is when scroll isn't selected
    
    "MovementLabel"
    {
        "ControlName" "Label"
        "fieldName" "MovementLabel"
        "xpos" "2"
        "ypos" "30"
        "labelText" "#MOM_RunFlag_Movement"
        "auto_wide_tocontents" "1"
    }
    
    "WOnly"
    {
        "ControlName" "ToggleButton"
        "fieldName" "WOnly"
        "xpos" "5"
        "ypos" "45"
        "wide"		"88"
		"tall"		"16"
        "labelText" "#MOM_RunFlag_WOnly"
        "auto_wide_tocontents" "1"
        "textAlignment"		"center"
        "command" "ToggleWOnly"
        "actionsignallevel" "2"
        "Font"			"BigNoodleSmall"
    }
    
    "HalfSideways"
    {
        "ControlName" "ToggleButton"
        "fieldName" "HalfSideways"
        "xpos" "5"
        "ypos" "65"
        "wide"		"88"
		"tall"		"16"
        "labelText" "#MOM_RunFlag_HalfSideways"
        "auto_wide_tocontents" "1"
        "textAlignment"		"center"
        "command" "ToggleHSW"
        "actionsignallevel" "2"
        "Font"			"BigNoodleSmall"
    }
    
    "Sideways"
    {
        "ControlName" "ToggleButton"
        "fieldName" "Sideways"
        "xpos" "5"
        "ypos" "85"
        "wide"		"88"
		"tall"		"16"
        "labelText" "#MOM_RunFlag_Sideways"
        "auto_wide_tocontents" "1"
        "textAlignment" "center"
        "command" "ToggleSideways"
        "actionsignallevel" "2"
        "Font"			"BigNoodleSmall"
    }
    
    "Backwards"
    {
        "ControlName" "ToggleButton"
        "fieldName" "Backwards"
        "xpos" "5"
        "ypos" "105"
        "wide"		"88"
		"tall"		"16"
        "labelText" "#MOM_RunFlag_Backwards"
        "auto_wide_tocontents" "1"
        "textAlignment" "center"
        "command" "ToggleBackwards"
        "actionsignallevel" "2"
        "Font"			"BigNoodleSmall"
    }
    
    "Bonus"
    {
        "ControlName" "ToggleButton"
        "fieldName" "Bonus"
        "xpos" "5"
        "ypos" "130"
        "wide"		"88"
		"tall"		"16"
        "labelText" "#MOM_RunFlag_Bonus"
        "auto_wide_tocontents" "1"
        "textAlignment" "center"
        "command" "ToggleBonus"
        "actionsignallevel" "2"
        "Font"			"BigNoodleSmall"
    }
	"TAS"
    {
        "ControlName" "ToggleButton"
        "fieldName" "TAS"
        "xpos" "5"
        "ypos" "155"
        "wide"		"88"
		"tall"		"16"
        "labelText" "#MOM_RunFlag_TAS"
        "auto_wide_tocontents" "1"
        "textAlignment" "center"
        "command" "ToggleTAS"
        "actionsignallevel" "2"
        "Font"			"BigNoodleSmall"
    }

	"Reset"
    {
        "ControlName" "Button"
        "fieldName" "Reset"
        "xpos" "5"
        "ypos" "190"
        "wide"		"88"
		"tall"		"16"
        "labelText" "#MOM_RunFlag_Reset"
        "auto_wide_tocontents" "1"
        "textAlignment" "center"
        "command" "ResetFlags"
        "actionsignallevel" "2"
        "Font"			"BigNoodleSmall"
    }
}