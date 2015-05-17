// Wird beim Start des Programms ausgeführt
$(document).ready(function(){
    // Liste alle Produkte im Shop auf
    $("#shop-items").html(mySqlObj.listAllProducts());

    // Inititalisiere leeren Warenkorb
    $("#product-display").html(mySqlObj.showCart());
});

// Click-Events
$(document).ready(function(){

    /* Cleaner Code:
     * mySqlObj.balanceChanged.connect($("#acc-balance .accdd").html(mySqlObj.getBalance()));
     * mySqlObj.purchaseDone.connect(getBalance());
     * mySqlObj.cartChanged.connect(showCart());
     */

    /* // nicht mehr benötigt
    $("#showCart").click(function(){
        $("#product-display").html(mySqlObj.showCart());
    });
    */

    $("#purchase").on('click', function(){
        if(mySqlObj.purchase()){
            $("#acc-balance .accdd").html(mySqlObj.getBalance());
            $("#acc-cart .accdd").html(mySqlObj.getCurrentCartValue());
            $("#product-display").html(mySqlObj.showCart());
            $("#shop-items").html(mySqlObj.listAllProducts());
        }
    });

    // Produkt in den Warenkorb legen
    $("#shop-items").on('click', '[id^=buyCartItem]', function(){
        var pid = $(this).attr("id").slice(11);
        var amount =  $("#cartItemAmount"+pid).val();
        var price = $(this).parent().siblings().eq(2).text();
        var title = $(this).parent().siblings().eq(1).text();
        if(amount > 0){
            mySqlObj.addToCart(pid, amount, price, title);
            $("#product-display").html(mySqlObj.showCart());
            var currentCartValue = mySqlObj.getCurrentCartValue();
            if(mySqlObj.getLogin()){
                var balance = mySqlObj.getBalance();
                if(balance < currentCartValue){
                    $("#acc-cart .accdd").addClass("red-font");
                }
                else{
                    $("#acc-cart .accdd").removeClass("red-font");
                }
            }
            $("#acc-cart .accdd").html(currentCartValue);
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
             $("#product-display").html(mySqlObj.showCart());
            $("#acc-cart .accdd").html(mySqlObj.getCurrentCartValue());
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
        // alert("Der eingegebene Username war: "+username+" Das eingegebene Passwort war: "+password);
        mySqlObj.login(username, password);
        if(mySqlObj.getLogin()){

           $("#login-form").toggleClass("active inactive");
           $("#logout-form").toggleClass("active inactive");
           $("#acc-username .accdd").html(mySqlObj.getUsername());
           $("#acc-balance .accdd").html(mySqlObj.getBalance());

           var currentCartValue = mySqlObj.getCurrentCartValue();
           var balance = mySqlObj.getBalance();
           if(balance < currentCartValue){
               $("#acc-cart .accdd").addClass("red-font");
           }
           else{
               $("#acc-cart .accdd").removeClass("red-font");
           }
           $("#acc-cart .accdd").html(mySqlObj.getCurrentCartValue());

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
        mySqlObj.refillBalance(amount);
        var currentCartValue = mySqlObj.getCurrentCartValue();
        var balance = mySqlObj.getBalance();
        if(balance < currentCartValue){
            $("#acc-cart .accdd").addClass("red-font");
        }
        else{
            $("#acc-cart .accdd").removeClass("red-font");
        }
        $("#acc-balance .accdd").html(balance);
    });

    // Usertabelle ausgeben
    $("#listAllUsers").click(function(){
        $("#user-administration").html(mySqlObj.listAllUsers());
    });

    // Alles auswählen Checkboxen
    $(":checkbox.selectAll").change(function(){
        var name = $(this).attr("name");
        alert("name!");
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
// Sortierfunktionen
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
        var $controls = $table.find("th[data-sort]");
        var rows = $tbody.find("tr:not(.no-sort)").toArray();

        var $header = $(this);
        var order = $header.data("sort");
        var column;

        // Testfunktion
        // alert("$table:"+$table+"\n$tbody:"+$tbody+"\nrows:"+rows+"\n$header:"+$header+"\norder:"+order);

        if($header.is(".ascending")||$header.is(".descending")){
            $header.toggleClass("ascending descending");
            $tbody.append(rows.reverse());
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

                $tbody.append(rows);
            }
        }
    });
    // Filterfunktion
});
