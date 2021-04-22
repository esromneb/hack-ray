function globalLoad(name) {

  const got = localStorage.getItem(name);
  // let o = JSON.parse(got);

  $('#jsonscene').val(got);

    // return o;

}

//need some sort of a mutex here
function globalSave(name) {

  const scene = $('#jsonscene').val();

  // console.log(txt);
  localStorage.setItem(name, scene);
}

