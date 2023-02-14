/*
 * File:      switches.js
 * Authors:   Luke de Munk
 * 
 * Used to display and handle checkboxes.
 */

/**************************************************************************/
/*!
  @brief    Runs when document is loaded.
*/
/**************************************************************************/
$(document).ready(function() {
    document.getElementById("cb_power").checked = power;
    document.getElementById("cb_inversed").checked = inverted;
});

/**************************************************************************/
/*!
  @brief    Sends power command to the display.
*/
/**************************************************************************/
function setPower() {
    power = document.getElementById("cb_power").checked ? 1 : 0;

    $.ajax({
        url: "/set_power",
        type: "get",
        data: {
            power: power
            },
        success: function(response) {},
        error: function(xhr) {}
    });
}

/**************************************************************************/
/*!
  @brief    Sends inverse command to the display.
*/
/**************************************************************************/
function setInversed() {
    inverted = document.getElementById("cb_inversed").checked ? 1 : 0;

    $.ajax({
        url: "/set_invert",
        type: "get",
        data: {
            inverted: inverted
            },
        success: function(response) {},
        error: function(xhr) {}
    });
}