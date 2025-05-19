package tadiran.webnla.accdashboard;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import jakarta.servlet.http.HttpServletRequest;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

import org.krysalis.jcharts.Chart;
import org.krysalis.jcharts.chartData.ChartDataException;
import org.krysalis.jcharts.encoders.PNGEncoder;
import tadiran.webnla.accdashboard.services.ChartService;

@CrossOrigin(origins = "*", maxAge = 3600)
@RestController
@RequestMapping("/auth")
public class ChartController {
    private static final Logger logger = LogManager.getLogger(ChartController.class);

    @Autowired
    private ChartService chartService;

    @GetMapping(value = "/chart", produces = MediaType.IMAGE_PNG_VALUE)
    public ResponseEntity<byte[]> getChart(HttpServletRequest request,
                                           @RequestParam(value = "type", required = false) String type,
                                           @RequestParam(value = "title", required = false) String title,
                                           @RequestParam(value = "keys", required = false) String keys,
                                           @RequestParam(value = "size", required = false) String size,
                                           @RequestParam(value = "refObjectName", required = false) String refObjectName) throws IOException {
        try {
            logger.info("get chart request");
            Chart chart = chartService.getChart(request);
            logger.info("response to chart request");
            if (chart != null) {
                ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
                PNGEncoder.encode(chart, outputStream);
                byte[] chartData = outputStream.toByteArray();
                return ResponseEntity.ok()
                        .contentType(MediaType.IMAGE_PNG)
                        .body(chartData);
            } else {
                return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
            }
        } catch (ChartDataException e) {
            logger.warn("Error generating chart due to bad request data: {}", e.getMessage());
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        } catch (Exception e) {
            logger.error("Error generating chart", e);
            return new ResponseEntity<>(HttpStatus.INTERNAL_SERVER_ERROR);
        }
    }
}
