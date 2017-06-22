(function () {








  window.setInterval(function () {
  
    getHVAC();
    getDryer();
    getRef();
    changeHVACWatts();
  }, 1500);


  function changeHVACWatts() {
    var hvacWatts = 0.31;

    $(".hvac-watts").html((getRandomArbitrary(0, 0.1) + hvacWatts).toFixed(2));
   
  }

  function changeRefWatts(dryoff) {
    var RefWatts = 0.39;
    if (dryoff != 0) {
      $(".ref-watts").html((getRandomArbitrary(0, 0.1) + RefWatts).toFixed(2));
    }
    else {
      $(".ref-watts").html(0);
    }

  }

  function changeDryerWatts(val) {
    
    var DryerWatts = 0.94;
    var dryerval = val/1000;
    var dryfinalVal = (DryerWatts+dryerval)+getRandomArbitrary(0, 0.09);
    $(".dryer-watts").html(dryfinalVal.toFixed(2));
  }

  function getRandomArbitrary(min, max) {
    return Math.random() * (max - min) + min;
  }

function changeTempIcon(temp){
   if(temp>28){
 $(".hvac-temp-icon").attr("src","main/IMG/Temperature-01-WF.png");
   }
   else{
     $(".hvac-temp-icon").attr("src","main/IMG/Temperature-01-WF-hot.png");
   }
   
   
}
  function getHVAC() {

    $.ajax({
      type: "get",
      //url: window.location.protocol + "//" + window.location.host +':3000/hvac?_sort=id&_order=desc&_limit=1',
      url: 'http://localhost:3000/hvac?_sort=id&_order=desc&_limit=1',

      cache: false,
      success: function (data) {
        console.log(data);
        $(".hvac-temp").html(data[0].temp);
        $(".hvac-humid").html(data[0].humid);
        changeTempIcon(data[0].temp);
      }
    });


  }

  function getDryer() {
    $.ajax({
      type: "get",
      //url: window.location.protocol + "//" + window.location.host +':3000/hvac?_sort=id&_order=desc&_limit=1',
      url: 'http://localhost:3000/ref-dry?_sort=id&_order=desc&_limit=1',

      cache: false,
      success: function (data) {
        console.log(data);
changeDryerWatts(data[0].degree);
      }
    });
  }

  function getRef() {
    $.ajax({
      type: "get",
      //url: window.location.protocol + "//" + window.location.host +':3000/hvac?_sort=id&_order=desc&_limit=1',
      url: 'http://localhost:3000/ref-dry?_sort=id&_order=desc&_limit=1',

      cache: false,
      success: function (data) {
        changeRefWatts(data[0].dryoff);
      }
    });
  }


}());