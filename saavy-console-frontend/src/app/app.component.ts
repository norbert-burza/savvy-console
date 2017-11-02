import { Component } from '@angular/core';
import { InputService } from './services/input.service';
import { HttpClient } from '@angular/common/http';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'app';

  content:string[] = [];

  consoleOutput:ConsoleOutput;

  constructor(public http : HttpClient) {
    this.content[0] = 'sample';
  }

  prepareSampleData() {
    var line:string = '';
  }

  myFunc(data) {
     console.log(data);
     if (data.keyCode == 13) {
       this.content.push('');
     }
    var character =  String.fromCharCode(data.keyCode);

    console.log(character);

    var req1:ConsoleInput = new ConsoleInput();
    req1.input = character;



    var req = this.http.post<ConsoleOutput>('http://localhost:8080/input', req1);
    req.subscribe(data => {
      this.consoleOutput = data;
    }, err => console.log('Something went wrong'));
    console.log(this.consoleOutput);
    this.content[this.content.length-1] += this.consoleOutput.output;
  }


}

class ConsoleOutput {
  output:string;
}

class ConsoleInput {
  input:string;
}