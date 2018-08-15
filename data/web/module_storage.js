var storageModule = (function () {
  // private

  (function initialize() {
    Window.localStorage.prototype.setObject = function(key, value) {
        this.setItem(key, JSON.stringify(value));
    }

    Window.localStorage.prototype.getObject = function(key) {
        var value = this.getItem(key);
        return value && JSON.parse(value);
    }

  })();

  return {
    // public
  };
}());
