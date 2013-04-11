#lang racket

(require racket/draw
         math/matrix)

(provide load-image-file
         raw-file->png)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; load-image-file.rkt
;;
;; The project must support three image formats: JPG, PNG, and GIF
;; (sans-animation). We rely on the racket image library to perform the image
;; loading.

(define (raw-file->png in out w h)
  (with-input-from-file in
    (with-output-to-file out
      (let ((bs (port->bytes)))
        (let* ((new-image
                (make-object bitmap%
                             ;; the width and height don't matter because they will be
                             ;; replaced by the dimensions of the image we load
                             1 1
                             ;; we are not loading a monochrome image
                             #f
                             ;; ignore alpha channel (transparency)
                             #f)))
          (send new-image set-argb-pixels
                0 0
                w h
                bs)
          (let ((write-successful?
                 (send new-image save-file
                       "out.png"
                       "png")))
            (if write-successful?
                new-image
                #f)))))))

(define (load-image-file fn)
  (bitmap%->bytes (filename->bitmap% fn)))

;; filename->bitmap% : Path -> (U False bitmap%)
;;
;; loads the file into a bitmap object, or, if that fails, returns false
(define (filename->bitmap% filename)
  (let* ((new-image
          (make-object bitmap%
                       ;; the width and height don't matter because they will be
                       ;; replaced by the dimensions of the image we load
                       1 1
                       ;; we are not loading a monochrome image
                       #f
                       ;; ignore alpha channel (transparency)
                       #f))
         (load-succesful?
          (send new-image load-file
                filename
                ;; we don't know the type of file, but definitely ignore the
                ;; alpha channel (transparency)
                'unknown)))
    (if load-succesful?
        new-image
        #f)))

;; bitmap%->bytes : bitmap% -> [Values Bytes Number Number]
;;
;; extracts the entire image as a sequence of bytes and returns those bytes as
;; well as the image width and height IN PIXELS (i.e. the byte matrix is (*
;; width 4) bytes wide).
(define (bitmap%->bytes bm)
  (let* ((width (send bm get-width))
         (height (send bm get-height))
         ;; unfortunately, there is no procedure to only get the rgb values, so
         ;; we need four bytes per pixel; we will later drop the alpha value
         (number-of-bytes-necessary (* width
                                       height
                                       4))
         (byte-array-of-pixels (make-bytes number-of-bytes-necessary)))
   (send bm get-argb-pixels
         ;; get everything from the top-left corner ...
         0 0
         ;; ... to the bottom right corner ...
         width height
         ;; ... and store it in here.
         byte-array-of-pixels)
   (with-output-to-file "out.raw"
     (lambda () (write-bytes byte-array-of-pixels)))))
