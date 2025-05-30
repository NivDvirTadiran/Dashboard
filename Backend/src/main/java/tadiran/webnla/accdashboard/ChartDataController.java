package tadiran.webnla.accdashboard;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import tadiran.webnla.accdashboard.payload.response.ChartDataResponse;
import tadiran.webnla.accdashboard.services.ChartDataService;

import jakarta.servlet.http.HttpServletRequest;

@CrossOrigin(origins = "*", maxAge = 3600)
@RestController
@RequestMapping("/auth")
public class ChartDataController {
    private static final Logger logger = LogManager.getLogger(ChartDataController.class);

    @Autowired
    private ChartDataService chartDataService;

    @GetMapping("/chart-data")
    public ResponseEntity<ChartDataResponse> getChartData(HttpServletRequest request,
                                                         @RequestParam(value = "type", required = false) String type,
                                                         @RequestParam(value = "title", required = false) String title,
                                                         @RequestParam(value = "keys", required = false) String keys,
                                                         @RequestParam(value = "refObjectName", required = false) String refObjectName) {
        try {
            logger.info("Get chart data request - type: {}, keys: {}, refObjectName: {}", type, keys, refObjectName);
            
            ChartDataResponse chartData = chartDataService.getChartData(request, type, title, keys, refObjectName);
            
            if (chartData != null) {
                logger.info("Response to chart data request - {} data points", chartData.getDatasets().size());
                return ResponseEntity.ok(chartData);
            } else {
                logger.warn("No chart data generated for request");
                return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
            }
        } catch (IllegalArgumentException e) {
            logger.warn("Error generating chart data due to bad request: {}", e.getMessage());
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        } catch (Exception e) {
            logger.error("Error generating chart data", e);
            return new ResponseEntity<>(HttpStatus.INTERNAL_SERVER_ERROR);
        }
    }
}