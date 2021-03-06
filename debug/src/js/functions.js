// Shop initialisieren ///////////////////////////////////////////////////////////////////

$(document).ready(function(){
    // Liste alle Produkte im Shop auf
    $("#shop-items").html(mySqlObj.listAllProducts());

    // Inititalisiere leeren Warenkorb
    $("#product-display").html(mySqlObj.showCart());
});

// Shopverwaltung
$(document).ready(function(){
    $("#user-administration").html(mySqlObj.listAllUsers());
    $("#ware-administration").html(mySqlObj.listAllProducts("checkboxes"));
});

// ------------------- //////////////////////////////////////////////////////////////////

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
    $("#purchase").on('click', function(){
        mySqlObj.purchase();
    });

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
    $("#product-display").on('click', '[id^=removeItem]', function(){
        var pid = $(this).attr("id").slice(10);
        var diff =  $("#itemAmount"+pid).val();
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

    // Verlassen-Button
    $("#quit").click(Qt.quit);

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

    // User registrieren
    $("#register").click(function(){
        var username = $("#rUsername").val();
        var password = $("#rPassword").val();
        var repeatedPassword = $("#rPasswordRepeated").val();
        // Testfunktion
        // alert("Der eingegebene Username war: "+username+" Das eingegebene Passwort war: "+password+" Das wiederholte Passwort war: "+repeatedPassword);
        mySqlObj.registerUser(username, password, repeatedPassword);
    });

    // Guthaben aufladen
    $("#refillBalance").click(function(){
        var amount = $("#amount").val();
        // Testfunktion
        // alert("Der eingegebene Betrag war: "+amount+".");
        mySqlObj.changeBalance(mySqlObj.getUid(),"add",amount);
    });

    // Warentabelle ausgeben
    $("#listAllProducts").click(function(){
        $("#ware-administration").html(mySqlObj.listAllProducts("checkboxes"));
    });

    // Usertabelle ausgeben
    $("#listAllUsers").on("click", function(){
        $("#user-administration").html(mySqlObj.listAllUsers());
    });

    // User Action Events ////////////////////////////////////////////////////////////

    // ids zu gecheckten Checkboxen ermitteln
    function getIds(obj){
        var $table = obj.parent().siblings().find("table");
        // alert($table.attr("id"));

        return $table.find('input:checkbox:checked').map(function(){
            return $(this).parent().next().text();
        }).toArray();
    }

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

    // User zum Admin ernennen
    $("#empowerUser").click(function(){
        userActionEvent(mySqlObj.empowerUser, $(this));
    });

    // User Admin-Status entziehen
    $("#disempowerUser").click(function(){
        userActionEvent(mySqlObj.disempowerUser, $(this));
    });

    // Account blocken
    $("#blockAccount").click(function(){
        var duration = $(this).find("#acc-admin-duration").val();

        if(duration === "permanent"){
             userActionEvent(mySqlObj.blockAccountPermanently, $(this));
        }
        else{
            userActionEvent(mySqlObj.blockAccount, $(this), duration);
        }
    });

    // Account entblocken
    $("#unblockAccount").click(function(){
        userActionEvent(mySqlObj.unblockAccount, $(this));
    });

    // Account terminieren
    $("#terminateAccount").click(function(){
        userActionEvent(mySqlObj.terminateAccount, $(this));
    });

    // User-Guthaben ändern
    $("#changeBalance").click(function(){
        var mode = $("#balance-admin-mode").val();
        var amount = $("#balance-admin-amount").val();

        // Testfunktion
        //alert(mode+","+amount);

        userActionEvent(mySqlObj.changeBalance, $(this).closest("table"), mode, amount);
        $("#user-administration").html(mySqlObj.listAllUsers());
    });

    // Zwingen den User sein Passwort zu ändern
    $("#changePassword").click(function(){
        var mode = $("#password-admin-mode").val();
        // Testfunktion
        alert(mode);
        userActionEvent(mySqlObj.changeBalance, $(this).closest("table"), mode);
    });


    // Warenbestand ändern
    $("#changeStock").click(function(){
        var mode = $("#stock-admin-mode").val();
        var amount = $("#stock-admin-amount").val();

        // Testfunktion
        //alert(mode+","+amount);

        userActionEvent(mySqlObj.changeStock, $(this).closest("table"), mode, amount);
        $("#ware-administration").html(mySqlObj.listAllProducts("checkboxes"));
    });

    // Warenpreis ändern
    $("#changeWarePrice").click(function(){
        var mode = $("#price-admin-mode").val();
        var amount = $("#price-admin-amount").val();

        // Testfunktion
        //alert(mode+","+amount);

        userActionEvent(mySqlObj.changeWarePrice, $(this).closest("table"), mode, amount);
        $("#ware-administration").html(mySqlObj.listAllProducts("checkboxes"));
    });

    // Warenmarge ändern
    $("#changeMargin").click(function(){
        var mode = $("#margin-admin-mode").val();
        var amount = $("#margin-admin-amount").val();

        // Testfunktion
        //alert(mode+","+amount);

        userActionEvent(mySqlObj.changeMargin, $(this).closest("table"), mode, amount);
        $("#ware-administration").html(mySqlObj.listAllProducts("checkboxes"));
    });

    // Alles auswählen Checkboxen
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
// Sortierfunktion
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

$(document).ready(function(){
    $("[id^=automCmd]").on("click", function(){
        // alert("Succesfull call!");
        var input = $(this).prop("id").slice(8);
        automaton.transitions(input);
    });
});

// ------------------- //////////////////////////////////////////////////////////////////

// Qt-Funktionen-Connect ////////////////////////////////////////////////////////////////

function cartChangedEvent(){
    alert("The cart changed!"+" Current Value:"+mySqlObj.getCurrentCartValue());
    refreshAccountData();
}

function priceChangedEvent(){
    alert("The price has changed!");
}

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

// ------------------- //////////////////////////////////////////////////////////////////
