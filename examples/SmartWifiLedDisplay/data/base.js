/*
 * File:      base.js
 * Authors:   Luke de Munk
 * 
 * General javascript for web page.
 */

/**************************************************************************/
/*!
  @brief    Runs when document is loaded.
*/
/**************************************************************************/
$(document).ready(function() {
    updateButtons();
});

/**************************************************************************/
/*!
  @brief    Sends the intensity to the display.
*/
/**************************************************************************/
$("#intensity").change(
    function() {
        intensity = $(this).val();
        $.ajax({
            url: "/set_intensity",
            type: "get",
            data: {
                intensity: intensity
                },
            success: function(response) {},
            error: function(xhr) {}
        });
    }
);

/**************************************************************************/
/*!
  @brief    Sends the screen 1 command to the display.
*/
/**************************************************************************/
$("#screen1Btn").click(
    function() {
        screen = 0;
        setScreen();
    }
);

/**************************************************************************/
/*!
  @brief    Sends the screen 2 command to the display.
*/
/**************************************************************************/
$("#screen2Btn").click(
    function() {
        screen = 1;
        setScreen();
    }
);

/**************************************************************************/
/*!
  @brief    Sends the screen 3 command to the display.
*/
/**************************************************************************/
$("#screen3Btn").click(
    function() {
        screen = 2;
        setScreen();
    }
);

/**************************************************************************/
/*!
  @brief    Sends the screen select command to the display.
*/
/**************************************************************************/
function setScreen() {
    $.ajax({
        url: "/set_screen",
        type: "get",
        data: {
            screen: screen
            },
        success: function(response) {},
        error: function(xhr) {}
    });
    updateButtons();
}

/**************************************************************************/
/*!
  @brief    Updates css classes for the selected button.
*/
/**************************************************************************/
function updateButtons() {
    if (screen == 0) {
        document.getElementById("screen1Btn").className = "button_sel";
        document.getElementById("screen2Btn").className = "button";
        document.getElementById("screen3Btn").className = "button";
    } else if (screen == 1) {
        document.getElementById("screen1Btn").className = "button";
        document.getElementById("screen2Btn").className = "button_sel";
        document.getElementById("screen3Btn").className = "button";
    } else if (screen == 2) {
        document.getElementById("screen1Btn").className = "button";
        document.getElementById("screen2Btn").className = "button";
        document.getElementById("screen3Btn").className = "button_sel";
    }
}