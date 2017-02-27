using System.Web.Mvc;

namespace DataCollector.Controllers
{
  public class HomeController : Controller
  {
    public ActionResult Index()
    {
      ViewBag.Title = "Home Page";

      return View();
    }

    public ActionResult Temperature()
    {
      ViewBag.Title = "Temperature";

      return View();
    }

    public ActionResult Light()
    {
      ViewBag.Title = "Light";

      return View();
    }
  }
}
