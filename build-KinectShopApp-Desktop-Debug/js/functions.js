// Shop initialisieren ///////////////////////////////////////////////////////////////////

/*
  * @brief Fügt bei #shop-items den return von listAllProducts ein
  * @return void
  */

$(document).ready(function(){
    // Liste alle Produkte im Shop auf
    $("#shop-items").html(mySqlObj.listAllProducts());

    // Inititalisiere leeren Warenkorb
    $("#product-display").html(mySqlObj.showCart());
});

/*
  * @brief Fügt bei #user-administration den return von listAllUsers ein
  * Fügt bei #ware-administration den return von listAllProducts ein
  * @return void
  */

// Shopverwaltung
$(document).ready(function(){
    $("#user-administration").html(mySqlObj.listAllUsers());
    $("#ware-administration").html(mySqlObj.listAllProducts("checkboxes"));
});

// ------------------- //////////////////////////////////////////////////////////////////

/*
  * @brief Funktion zur Aktualisierung der Account-Anzeige
  * (Name, Geld, Admin/Kunde, Warenkorbwert)
  * wird von diversen eventbasierten Funktionen aufgerufen.
  * @return void
  */

function refreshAccountData(){
    var currentBalance = mySqlObj.getBalance();
    var currentCartValue = mySqlObj.getCurrentCartValue();

    $("#acc-balance .accdd").html(currentBalance+"("+(currentBalance-currentCartValue)+")");
    $("#acc-cart .accdd").html(currentCartValue);
    $("#product-display").html(mySqlObj.showCart());
    $("#shop-items").html(mySqlObj.listAllProducts());
    if(mySqlObj.getLogin()){
        var balance = mySqlObj.getBalance();
        if(balance < currentCartValue){
            $("#acc-cart .accdd").addClass("red-font");
        }
        else{
            $("#acc-cart .accdd").removeClass("red-font");
        }
    }
}

// Click-Events
$(document).ready(function(){

    /*
      * @brief Beim Klick auf #purchase wird die
      * Funktion purchase() aufgerufen.
      * @return void
      */

    $("#purchase").on('click', function(){
        mySqlObj.purchase();
    });

    /*
      * @brief Beim Klick auf #^buyCartItem wird die
      * Funktion addToCart(pid, amount, price, title) aufgerufen (delegiert).
      * Die angegebenen Parameter beziehen sich nicht auf die Funktion,
      * sondern auf die in der Funktion aufgerufenen FUnktion.
      * @param pid ist der Produkt-Id
      * @param amount ist die dem Warenkorb hinzugefügte Menge
      * @param price ist der Preis einer Einheit dieses Produkts
      * @param title ist der Name des Produkts
      * @return void
      */
    // Produkt in den Warenkorb legen
    $("#shop-items").on('click', '[id^=buyCartItem]', function(){
        var pid = $(this).attr("id").slice(11);
        var amount =  $("#cartItemAmount"+pid).val();
        var price = $(this).parent().siblings().eq(2).text();
        var title = $(this).parent().siblings().eq(1).text();
        if(amount > 0){
            mySqlObj.addToCart(pid, amount, price, title);
            // Testfunktion
            alert("Das Produkt ist: "+title+"\nDie Produkt ID ist: "+pid+"\nDie Menge ist: "+amount+"\nDer Preis pro Stück ist: "+price);
        }
        else{
            alert("Die eingekaufte Menge muss größer als 0 sein.");
        }
    });

    // Produktanzahl im Warenkorb reduzieren
    /*
      * @brief Beim Klick auf #^removeItem wird die
      * Funktion changeAmount(pid, diff, mode) mit mode="sub" aufgerufen (delegiert).
      * Die angegebenen Parameter beziehen sich nicht auf die Funktion,
      * sondern auf die in der Funktion aufgerufenen FUnktion.
      * @param pid Produkt-ID
      * @param diff Anzahl der zu entfernenden Produkteinheiten
      * @param mode Modus, hier nur sub
      * @return void
      */

    $("#product-display").on('click', '[id^=removeItem]', function(){
        var pid = $(this).attr("id").slice(10);
        var diff =  $("#itemAmount"+pid).val();
        if (isNaN(diff) === true){alert("Error: NaN!"); return};
        var mode = "sub";
        if(diff > 0){
            // Testfunktion
            // alert(pid+" "+diff+" "+mode);
            mySqlObj.changeAmount(pid, diff, mode);

            // Testfunktion
            // alert("\nDie Produkt ID ist: "+pid+"\nDie Menge ist: "+diff+"\nDer Modus ist: "+mode);
        }
        else{
            alert("Die zu entfernende Menge muss größer als 0 sein.");
        }
    });

    /*
      *@brief Logout-Funktion wird beim Klick auf das X aufgerufen.
      *@return void.
    */
    // Verlassen-Button
    $("#quit").click(Qt.quit);

    /*
      * @brief Beim Klick auf #^login wird die
      * Funktion login(usernamem password) aufgerufen. Anschließend wird der
      * erfolgreiche Login überprüft und ggf. Anpassungen am UI vorgenommen.
      * Die angegebenen Parameter beziehen sich nicht auf die Funktion,
      * sondern auf die in der Funktion aufgerufenen FUnktion.
      * @param username Benutzername
      * @param password Passwort
      * @return void
      */
    // Login
    $("#login").click(function(){
        var username = $("#username").val();
        var password = $("#password").val();

        // Testfunktion
        //alert("Der eingegebene Username war: "+username+" Das eingegebene Passwort war: "+password);

        mySqlObj.login(username, password);
        if(mySqlObj.getLogin()){ // if-Abfrage Relikt aus vorheriger Version, eigentlich nicht benötigt!!
           $("#login-form").toggleClass("active inactive");
           $("#logout-form").toggleClass("active inactive");
           $("#acc-username .accdd").html(mySqlObj.getUsername());

           if(mySqlObj.getIsAdminLoggedIn()){
               $("#acc-isAdmin .accdd").html("Admin");
               $("li[title='admin-area']").toggleClass("active inactive");
           }
           else{
               $("#acc-isAdmin .accdd").html("Kunde");
           }
        }
    });

    /*
     * @brief Logout-Funktion (wird beim Klick auf #logout aufgerufen)
     * @return void
     */
    // Logout
    $("#logout").click(function(){
        mySqlObj.logout();
        if(!mySqlObj.getLogin()){
           $("#login-form").toggleClass("active inactive");
           $("#logout-form").toggleClass("active inactive");
           $("#acc-username .accdd").html(" --- ");
           $("#acc-balance .accdd").html(" --- ");
           $("#acc-isAdmin .accdd").html(" --- ");

            // Nicht toggle, weil nicht klar ist ob Kunde oder Admin eingeloogt war
           $("li[title='admin-area']").addClass("inactive");
           $("li[title='admin-area']").removeClass("active");
        }
        $("#navStart").click().addClass("active").removeClass("inactive");
    });

    /*
      * @brief Beim Klick auf #register wird die
      * Funktion login(usernamem password, repeatedPassword) aufgerufen.
      * Die angegebenen Parameter beziehen sich nicht auf die Funktion,
      * sondern auf die in der Funktion aufgerufenen Funktion.
      * @param username Benutzername
      * @param password Passwort
      * @param repeatedPassword wiederholtes Passwort
      * @return void
      */
    // User registrieren
    $("#register").click(function(){
        var username = $("#rUsername").val();
        var password = $("#rPassword").val();
        var repeatedPassword = $("#rPasswordRepeated").val();
        // Testfunktion
        // alert("Der eingegebene Username war: "+username+" Das eingegebene Passwort war: "+password+" Das wiederholte Passwort war: "+repeatedPassword);
        mySqlObj.registerUser(username, password, repeatedPassword);
    });

    /*
     * @brief Beim Klick auf #refillBalance wird die Funktion
     * changeBalance(id,mode,amount) aufgerufen
     * Die angegebenen Parameter beziehen sich nicht auf die Funktion,
     * sondern auf die in der Funktion aufgerufenen Funktion.
     * @param id User-Id
     * @param mode hier nur "add" zum hinzufügen von Userguthaben
     * @param amount Geldmenge
     * @return void
     */
    // Guthaben aufladen
    $("#refillBalance").click(function(){
        var amount = $("#amount").val();
        if (isNaN(amount) === true){alert("Error: NaN!"); return};
        // Testfunktion
        // alert("Der eingegebene Betrag war: "+amount+".");
        mySqlObj.changeBalance(mySqlObj.getUid(),"add",amount);
    });

    /*
     * @brief Beim Klick auf #listAllProducts wird die Funktion
     * listAllProducts() aufgerufen
     * Die angegebenen Parameter beziehen sich nicht auf die Funktion,
     * sondern auf die in der Funktion aufgerufenen Funktion.
     * @return QString (Produkttabelle in HTML-Format)
     */
    // Warentabelle ausgeben
    $("#listAllProducts").click(function(){
        $("#ware-administration").html(mySqlObj.listAllProducts("checkboxes"));
    });

    /*
     * @brief Beim Klick auf #listAllUsers wird die Funktion
     * listAllUsers() aufgerufen
     * Die angegebenen Parameter beziehen sich nicht auf die Funktion,
     * sondern auf die in der Funktion aufgerufenen Funktion.
     * @return QString (Usertabelle in HTML-Format)
     */
    // Usertabelle ausgeben
    $("#listAllUsers").on("click", function(){
        $("#user-administration").html(mySqlObj.listAllUsers());
    });

    // User Action Events ////////////////////////////////////////////////////////////

    // ids zu gecheckten Checkboxen ermitteln
    /*
     * @brief Liefert den Inhalt von von Tabellenzellen direkt neben gecheckten
     * Checkboxen zurück (ids)
     * @param ein Objekt in der "Nähe" eines <table> (siehe Funktion)
     * @return Array mit ids
     */
    function getIds(obj){
        var $table = obj.parent().siblings().find("table");
        // alert($table.attr("id"));

        return $table.find('input:checkbox:checked').map(function(){
            return $(this).parent().next().text();
        }).toArray();
    }

    /*
     * @brief Codesparfunktion! Da hier viele Funktionen hier ähnlich aufgabeuat
     * sind hier eine Funktion die eine Callback, ein Objekt, sowie bis zu zwei
     * Funktionsparameter annimmt.
     * @param callback Rückruffunktion
     * @param obj Objekt (hier meist Objekt in der "Nähe" einer Tabelle), auf die sich die Funktion bezieht
     * @param argument1 (optionales) erstes Funktionsargument des Callbacks
     * @param argument2 (optionales) zweites Funktionsargument des Callbacks
     * @return void
     */
    function userActionEvent(callback, obj, argument1, argument2){
        var ids = getIds(obj);
        if(typeof argument2 !== 'undefined'){
            for(i=0; i < ids.length; i++){
                //alert(ids[i]+","+argument1+","+argument2);
                callback(ids[i], argument1, argument2);
            }
            return;
        }
        if(typeof argument1 !== 'undefined'){
            for(i=0; i < ids.length; i++){
                callback(ids[i], argument1);
            }
            return;
        }
        for(i=0; i < ids.length; i++){
            callback(ids[i]);
        }

        alert("Folgende Accounts wurden bearbeitet: " + ids);

        $("#user-administration").html(mySqlObj.listAllUsers());
    }

    /*
     * @brief Beim Klick auf #empowerUser wird die (indirekt) Funktion
     * empowerUser() aufgerufen, genauere Beschreibung im C++ Teil (analog zu
     * fast allen anderen Funktionen von oben), macht User zum Admin.
     * @return void
     */
    // User zum Admin ernennen
    $("#empowerUser").click(function(){
        userActionEvent(mySqlObj.empowerUser, $(this));
    });

    /*
     * @brief Beim Klick auf #disempowerUser wird die (indirekt) Funktion
     * disempowerUser() aufgerufen, genauere Beschreibung im C++ Teil (analog zu
     * fast allen anderen Funktionen von oben), entzieht User Adminprivilegien.
     * @return void
     */
    $("#disempowerUser").click(function(){
        userActionEvent(mySqlObj.disempowerUser, $(this));
    });

    /*
     * @brief ... blockt Account.
     * @return void
     */
    $("#blockAccount").click(function(){
        var duration = $(this).find("#acc-admin-duration").val();

        if(duration === "permanent"){
             userActionEvent(mySqlObj.blockAccountPermanently, $(this));
        }
        else{
            userActionEvent(mySqlObj.blockAccount, $(this), duration);
        }
    });

    /*
     * @brief ... entblockt Account.
     * @return void
     */
    // Account entblocken
    $("#unblockAccount").click(function(){
        userActionEvent(mySqlObj.unblockAccount, $(this));
    });

    /*
     * @brief ... löscht Account.
     * @return void
     */
    // Account terminieren
    $("#terminateAccount").click(function(){
        userActionEvent(mySqlObj.terminateAccount, $(this));
    });

    /*
     * @brief analog zu refillBalance (aber nur Admins haben Zugriff).
     * @return void
     */
    // User-Guthaben ändern
    $("#changeBalance").click(function(){
        var mode = $("#balance-admin-mode").val();
        var amount = $("#balance-admin-amount").val();
        if (isNaN(amount) === true){alert("Error: NaN!"); return};

        // Testfunktion
        //alert(mode+","+amount);

        userActionEvent(mySqlObj.changeBalance, $(this).closest("table"), mode, amount);
        $("#user-administration").html(mySqlObj.listAllUsers());
    });

    /*
     * @brief ... zwingt den User sein Passwort zu ändern (nur Admins).
     * @return void
     */
    $("#changePassword").click(function(){
        var mode = $("#password-admin-mode").val();
        // Testfunktion
        alert(mode);
        userActionEvent(mySqlObj.changeBalance, $(this).closest("table"), mode);
    });


    /*
     * @brief ... verändert Warenbestand (nur Admins).
     * @return void
     */
    $("#changeStock").click(function(){
        var mode = $("#stock-admin-mode").val();
        var amount = $("#stock-admin-amount").val();
        if (isNaN(amount) === true){alert("Error: NaN!"); return};

        // Testfunktion
        //alert(mode+","+amount);

        userActionEvent(mySqlObj.changeStock, $(this).closest("table"), mode, amount);
        $("#ware-administration").html(mySqlObj.listAllProducts("checkboxes"));
    });

    /*
     * @brief ... verändert Preise (nur Admins).
     * @return void
     */
    $("#changeWarePrice").click(function(){
        var mode = $("#price-admin-mode").val();
        var amount = $("#price-admin-amount").val();
        if (isNaN(amount) === true){alert("Error: NaN!"); return};

        // Testfunktion
        //alert(mode+","+amount);

        userActionEvent(mySqlObj.changeWarePrice, $(this).closest("table"), mode, amount);
        $("#ware-administration").html(mySqlObj.listAllProducts("checkboxes"));
    });

    /*
     * @brief ... verändert Gewinnmarge (nur Admins).
     * @return void
     */
    $("#changeMargin").click(function(){
        var mode = $("#margin-admin-mode").val();
        var amount = $("#margin-admin-amount").val();
        if (isNaN(amount) === true){alert("Error: NaN!"); return};

        // Testfunktion
        //alert(mode+","+amount);

        userActionEvent(mySqlObj.changeMargin, $(this).closest("table"), mode, amount);
        $("#ware-administration").html(mySqlObj.listAllProducts("checkboxes"));
    });

    /*
     * @brief ... wählt alle Checkboxen des nächsten <table> aus (nur Admins, delegiert).
     * @return void
     */
    $("body").on("change", ".selectAll", function(){
        var cid = $(this).prop("id");
        if(this.checked) {
            $(":checkbox[name^="+cid+"]").each(function() {
                this.checked = true;
            });
        }
        else{
            $(":checkbox[name^="+cid+"]").each(function() {
                this.checked = false;
            });
        }
    });

    /*
     * @brief ... sorgt dafür, dass nur der aktuell in der Navigation ausgewählte
     * Menüpunkt auf der rechten Seite angezeigt wird.
     * @return void
     */
    // Nur ausgewählten Menüpunkt anzeigen
    $("#left-nav ul li[title]").click(function(){
      var x = $(this).attr("title");
      $("#left-nav ul li[title]").removeClass("current");
      $(this).addClass("current");
      $("#content .section.active").toggleClass("active inactive");
      $("#" + x).toggleClass("active inactive");
    });
});

// Hilfsfunktionen
/*
 * @brief ... Sortierfunktion für Tabelle (delegiert)
 * @return void
 */
$(document).ready(function(){
    // Verschiedene Datentypen miteinander vergleichen
    var compare = {
        name: function(a,b){
            if(a < b){
                return -1;
            }
            else{
                return a > b ? 1 : 0;
            }
        },
        number: function(a, b){
            return a-b;
        }
    }

    // Sortierfunktion
    $("body").on("click", ".sortable th[data-sort]", function(){
        var $table = $(this).closest(".sortable");
        var $tbody = $table.find("tbody");
        var $controls = $table.find("th");
        var rows = $tbody.find("tr:not(.no-sort)").toArray();

        var $header = $(this);
        var order = $header.data("sort");
        var column;

        // Testfunktion
        // alert("$table:"+$table+"\n$tbody:"+$tbody+"\nrows:"+rows+"\n$header:"+$header+"\norder:"+order);

        if($header.is(".ascending")||$header.is(".descending")){
            $header.toggleClass("ascending descending");
            $tbody.prepend(rows.reverse());
        }
        else{
            $header.addClass("ascending");
            $header.siblings().removeClass("ascending descending");
            if(compare.hasOwnProperty(order)){
                column = $controls.index(this);

                rows.sort(function(a,b){
                  a = $(a).find("td").eq(column).text();
                  b = $(b).find("td").eq(column).text();
                  return compare[order](a,b);
                });

                $tbody.prepend(rows);
            }
        }
    });
});

// Automatenfunktionen //////////////////////////////////////////////////////////////////

// "Simulation der Spracherkennung"

/*
 * @brief ... Automatenbefehls# aus HTML# auslesen und dann entsprechende Transition ausführen.
 * @return void
 */
$(document).ready(function(){
    $("[id^=automCmd]").on("click", function(){
        // alert("Succesfull call!");
        var input = $(this).prop("id").slice(8);
        automaton.transitions(input);
    });
});

// ------------------- //////////////////////////////////////////////////////////////////

// Qt-Funktionen-Connect ////////////////////////////////////////////////////////////////

/*
 * @brief ... Alle folgenden Funktionen werden durch entsprechende gleichnamige SIGNALS()
 * im cpp-Teil aufgerufen, siehe connects weiter unten!
 * @return void
 */
function cartChangedEvent(){
    alert("The cart changed!"+" Current Value:"+mySqlObj.getCurrentCartValue());
    refreshAccountData();
}

function priceChangedEvent(){
    alert("The price has changed!");
}alert("The state has changed!");

function stockChangedEvent(){
    alert("The stock has changed!");
}

function balanceChangedEvent(){
    alert("The balance has changed!");
    refreshAccountData();
}

function marginChangedEvent(){
    alert("The margin of a product has been changed.");
}

function revenueMadeEvent(){
    alert("Revenue has been made.");
}

function profitMadeEvent(){
    alert("Profit has been made.");
}

function purchaseDoneEvent(){
    alert("The purchase has been completed!");
    $("#ware-administration").html(mySqlObj.listAllProducts("checkboxes"));
}

function userLoggedInEvent(){
    alert("The user "+ mySqlObj.getUsername()+" has logged in!");
    refreshAccountData();
}

function adminLoggedInEvent(){
    alert("The user is an administrator!");
}

function userLoggedOutEvent(){
    alert("The user has logged out!");
}

function stateChangedEvent(){
    alert("The state of the automaton has changed!");
    $("#automaton-status").html(automaton.updateStatusViewer());
}

function idChangedEvent(){
    alert("The product id saved in the automaton has changed!");
    $("#automaton-id").html(automaton.updateIdViewer());
}

function amountChangedEvent(){
    alert("The amount in the automaton has changed!");
    $("#automaton-amount").html(automaton.updateAmountViewer());
}

function productDetectedEvent(){
    $("#kinect-detected-id").html(kinect.updateDetectedPidViewer());
}

mySqlObj.cartChanged.connect(cartChangedEvent);
mySqlObj.priceChanged.connect(priceChangedEvent);
mySqlObj.stockChanged.connect(stockChangedEvent);
mySqlObj.balanceChanged.connect(balanceChangedEvent);
mySqlObj.marginChanged.connect(marginChangedEvent);
mySqlObj.revenueMade.connect(revenueMadeEvent);
mySqlObj.profitMade.connect(profitMadeEvent);
mySqlObj.purchaseDone.connect(purchaseDoneEvent);
mySqlObj.userLoggedIn.connect(userLoggedInEvent);
mySqlObj.adminLoggedIn.connect(adminLoggedInEvent);
mySqlObj.userLoggedOut.connect(userLoggedOutEvent);

automaton.stateChanged.connect(stateChangedEvent);
automaton.idChanged.connect(idChangedEvent);
automaton.amountChanged.connect(amountChangedEvent);

kinect.kinectProductDetected.connect(productDetectedEvent);

// ------------------- //////////////////////////////////////////////////////////////////
