/*
 * Copyright 2013 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

// helper to populate the form
function go() {
    var el = document.getElementsByClassName('app-input');
    for (var i = 0; i < el.length; i++) {
        el[i].value = 'asdf';
    }
}

function isEmpty(obj) {
    return Object.keys(obj).length === 0;
}

function getFoursquareId() {
    var pageUrl = document.getElementById('foursquare').value;
    pageUrl = pageUrl.substring(pageUrl.lastIndexOf("/"));
    pageUrl = pageUrl.replace("/", "");
    console.log(pageUrl);

    var xmlhttp;

    if (window.XMLHttpRequest) {
        xmlhttp = new XMLHttpRequest();

        xmlhttp.onreadystatechange = function() {
            if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                var json = JSON.parse(xmlhttp.responseText);
                console.log(json);


                var id = json.response.results[0].venue.id;
                document.getElementById('foursquare').value = id;
            }
        };

        var timestamp = getCurrentDate();
        xmlhttp.open("GET", "https://api.foursquare.com/v2/users/search?twitter=" + pageUrl + "&oauth_token=12KDAZVCKOH24T4EIHMGO1B13XUFO12L2EYNJA4OZFKAMIKK&v=" + timestamp, true);
        xmlhttp.send();
    }
}

// helper function for getting the current date, in the format foursquare is expecting it for our requests
function getCurrentDate() {
    var d = new Date();
    var year = d.getFullYear();
    var month = ("0" + (d.getMonth() + 1)).slice(-2);
    var day = ("0" + d.getDate()).slice(-2);
    var theDate = year + month + day;
    return theDate;
}

function getFacebookId() {
    var pageUrl = document.getElementById('facebook').value;
    pageUrl = pageUrl.substring(pageUrl.lastIndexOf("/"));
    var xmlhttp;

    if (window.XMLHttpRequest) {
        xmlhttp = new XMLHttpRequest();

        xmlhttp.onreadystatechange = function() {
            if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                var json = JSON.parse(xmlhttp.responseText);
                var id = json.id;
                document.getElementById('facebook').value = id;
            }
        };

        xmlhttp.open("GET", "http://graph.facebook.com" + pageUrl, true);
        xmlhttp.send();
    }
}



// initialization
var init = function() {
    addTab();
};

// show css textarea
var showCSS = function() {
    var el = document.getElementById('cssOverrides');
    el.style.display = 'block';
};

// show settings div
var showSettings = function() {
    var el = document.getElementById('settings');
    el.style.display = 'block';
};

// generate unique uuid for bbm
var getUUID = function() {
    var d = new Date().getTime();
    var uuid = 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function(c) {
        var r = (d + Math.random() * 16) % 16 | 0;
        d = Math.floor(d / 16);
        return (c == 'x' ? r : (r & 0x7 | 0x8)).toString(16);
    });
    return uuid;
};

// add a new action item
var addActionItem = function() {
    var el = document.getElementById('actions-container');
    var template = $("#template-actions").html();
    $('#actions-container').append(_.template(template, {}));
};

// add a new tab
var addTab = function(hi) {
    var el = document.getElementById('tab-container');
    var template = $("#template-tab").html();
    $('#tab-container').append(_.template(template, {}));
};

// process the form
var processForm = function() {
    // // make the inputs aren't blank
    // var el = document.getElementsByClassName('app-input');
    // for (var i = 0; i < el.length; i++) {
    //     var _this = el[i].value;

    //     // these fields CAN be blank
    //     if (_this === '') {
    //         if (el[i].id !== 'twitter' || el[i].id !== 'facebook') {
    //             alert('something is blank that shouldn\'t be!');
    //             return false;
    //         }
    //     }
    // }

    // set bbm uuid
    var uuid = getUUID();

    // build the form with values
    var theForm = {
        name: document.getElementById('company').value,
        address: document.getElementById('address').value,
        phone: document.getElementById('phone').value,
        appID: document.getElementById('appID').value,

        bbmChannel: document.getElementById('bbmChannel').value,
        facebook: document.getElementById('facebook').value,
        foursquare: document.getElementById('foursquare').value,
        shareText: document.getElementById('shareText').value,
        twitter: document.getElementById('twitter').value,
        settings: {}
    };

    // settings - orientation - figure out which settings radio button is checked
    var radios = document.getElementsByName('orientation');
    for (var i = 0; i < radios.length; i++) {
        if (radios[i].checked) {
            theForm.settings['orientation'] = radios[i].value;
        }
    }

    // settings - pinch to zoom - figure out which settings radio button is checked
    var radios = document.getElementsByName('pinch');
    for (var i = 0; i < radios.length; i++) {
        if (radios[i].checked) {
            theForm.settings['pinchToZoom'] = radios[i].value;
        }
    }

    // did they enter an @ in their name, if so remove it.
    var _twitterUsername = theForm.twitter.charAt(0);
    if (_twitterUsername === '@') {
        theForm.twitter = theForm.twitter.substring(1);
    }

    // process the tabs
    var tabs = document.getElementsByClassName('tab');
    var tabsLength = tabs.length;

    var tmpTabsArray = [];

    for (var i = 0; i < tabsLength; i++) {
        var currentTab = tabs[i];
        var thisTab = {};

        // build the tab json
        for (var x = 0; x < currentTab.childNodes.length; x++) {
            var currentChild = currentTab.childNodes[x].id;

            // setup the json for the tab
            if (currentChild === 'tabName' || currentChild === 'tabIcon' || currentChild === 'tabURL') {
                var id = currentTab.childNodes[x].id;
                var value = currentTab.childNodes[x].value;

                if (value === '') {
                    console.log('empty, stopping');
                } else {

                    if (id === 'tabName') {
                        thisTab['name'] = value;
                    } else if (id === 'tabIcon') {
                        thisTab['icon'] = 'asset:///images/' + value;

                    } else if (id === 'tabURL') {
                        thisTab['url'] = value;
                    }
                }
            }
        }

        // console.log('--- tab ---');
        // console.log(thisTab);

        // build the json string
        tmpTabsArray.push(thisTab);
    }

    var tmpTabsJSON = JSON.stringify(tmpTabsArray);

    // loop through each action item
    var actions = document.getElementsByClassName('action');
    var actionsLength = actions.length;
    var tmpActionsArray = [];

    for (var i = 0; i < actionsLength; i++) {
        var currentAction = actions[i];
        var thisAction = {};

        for (var x = 0; x < currentAction.childNodes.length; x++) {
            var currentChild = currentAction.childNodes[x].id;
            if (currentChild === 'actionName' || currentChild === 'actionIcon' || currentChild === 'actionURL') {
                var id = currentAction.childNodes[x].id;
                var value = currentAction.childNodes[x].value;

                if (value === '') {
                    console.log('empty, stopping');

                } else {

                    if (id === 'actionName') {
                        thisAction['name'] = value;
                    } else if (id === 'actionIcon') {
                        thisAction['icon'] = 'asset:///images/' + value;

                    } else if (id === 'actionURL') {
                        thisAction['url'] = value;
                    }
                }
            }
        }

        // check for empty actions object
        if (!isEmpty(thisAction)) {
            // build the json string
            tmpActionsArray.push(thisAction);
        }

    }

    // for each tab, tidy up the JSON so it works fine in momentics
    // for (var i = 0; i < tmpTabsArray.length; i++) {
    //     console.log(tmpTabsArray);
    // }

    // add actions to each tab
    if (tmpActionsArray.length > 0) {
        for (var i = 0; i < tmpTabsArray.length; i++) {
            tmpTabsArray[i]['actions'] = tmpActionsArray;
        }
    }

    var tmpActionsJSON = JSON.stringify(tmpActionsArray);

    var finalJSON = {
        name: theForm.name,
        address: theForm.address,
        phone: theForm.phone,
        bbmUUID: uuid,
        bbmChannel: theForm.bbmChannel,
        facebook: theForm.facebook,
        foursquare: theForm.foursquare,
        twitter: theForm.twitter,
        shareText: theForm.shareText,
        tabs: tmpTabsArray,
        settings: theForm.settings
    };

    var theJSON = JSON.stringify(finalJSON);

    console.log('--- the final JSON ---');
    console.log(theJSON);
    var theCSS = document.getElementById('cssOverrides').value || null;

    // download wrapgen.json
    console.log('downloading....');
    var jsonBlob = new Blob([theJSON], {
        type: "text/plain;charset=utf-8"
    });
    saveAs(jsonBlob, "settings.json");

    // if css was entered - download wrapgen.css
    if (theCSS !== null) {
        console.log('downloading....');

        var cssBlob = new Blob([theCSS], {
            type: "text/plain;charset=utf-8"
        });

        setTimeout(function() {
            saveAs(cssBlob, "settings.css");
        }, 1000);
    }
};