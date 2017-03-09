#if defined(ENABLEWEBUPDATE)

void setupWebUpdate()
{
  httpUpdater.setup(&httpServer);
  httpServer.begin();
}

void disableWebUpdate()
{
  if (allowWebUpdate)
  {
    httpServer.stop();
    allowWebUpdate = false;
  }
}

#endif

